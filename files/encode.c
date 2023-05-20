#include "func.h"
#include "encode.h"

short put_char_freq_in_file(FILE *input_file, data *info)
{

    unsigned char c = 0;
    while (fread(&c, sizeof(char), 1, input_file) > 0) {
        info[c].charact = c;
        info[c].freq++;
    }

    short num_of_unique_char = 0;
    for (short i = 0; i < NB_MAX_CHAR; i++) {
        if (info[i].freq == 0) continue;
        num_of_unique_char++;
    }
    fclose(input_file);
    return num_of_unique_char;
}

hqueue *fill_queue_with_tree(hqueue *queue, data *array, short size)
{
    if (!queue){
        htree *node = NULL;
        for (short i = 0; i < size; i++) {
            node = tree_create(array[i].charact, array[i].freq);
            queue = queue_push(queue, node);
        }
        return queue;
    }
    return NULL;
}

htree *build_huffman_tree(hqueue *priority_queue)
{
    htree *node1 = NULL;
    htree *node2 = NULL;
    while (priority_queue && priority_queue->next) {
        node1 = priority_queue->tree; node2 = priority_queue->next->tree;
        priority_queue = queue_pop(priority_queue); 
        priority_queue = queue_pop(priority_queue);
        priority_queue = queue_push(priority_queue, tree_create_parent(node1, node2));
    }
    htree *root = priority_queue->tree;
    priority_queue = queue_pop(priority_queue);
    return root;
}

void put_huffman_code_in_info(htree *root, data *info, unsigned bin_code, unsigned nbits)
{
    if (!root->left && !root->right) {
        info[root->charact].hcode = bin_code;
        info[root->charact].nbits = nbits;
        return;
    }
    /* Сдвигаем код влево на единицу и прибавляем 0(ничего не делаем после сдвига) */
    if (root->left){     
        put_huffman_code_in_info(root->left, info, (bin_code << 1), nbits + 1);
    }
    /* Сдвигаем код влево на единицу и прибавляем 1 */
    if (root->right){
        put_huffman_code_in_info(root->right, info, (bin_code << 1) + 1, nbits + 1);
    }
}

void write_info_in_binary_file(FILE *output_file, data *info, short size)
{
    fwrite(&size, sizeof(short), 1, output_file);
    for (short i = 0; i < NB_MAX_CHAR; i++){
        if (info[i].freq == 0) continue;
        fwrite(&info[i].charact, sizeof(char), 1, output_file);
        fwrite(&info[i].freq, sizeof(unsigned), 1, output_file);    
    }
}

void encoding(const char *inputname, const char *outputname, data *info, short size)
{
    float nbits_input = 0, nbits_output = 0; 
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    input_file = fopen(inputname, "r");
    output_file = fopen(outputname, "wb");
    write_info_in_binary_file(output_file, info, size);
    unsigned char read_char = 0;
    unsigned char c_bin = 0;
    unsigned int buffer = 0, buffer_size = 0; 
    while (fread(&read_char, sizeof(char), 1, input_file) > 0){
        buffer = (buffer << info[read_char].nbits);
        buffer = (buffer | info[read_char].hcode);
        buffer_size += info[read_char].nbits;

        while (buffer_size >= 8){
            
            buffer_size -= 8;
            c_bin = (buffer >> buffer_size);
            fwrite(&c_bin, sizeof(char), 1, output_file);
            nbits_output += 8;
        }

        nbits_input += 8;
    }

    if (buffer_size && buffer_size < 8) {
        c_bin = (buffer << (8-buffer_size));
        for (int i = 0; i < 8; i++){
        }
        fwrite(&c_bin, sizeof(char), 1, output_file);
        nbits_output += 8;
    }
    printf("Count of the number of bits before encode = %.2lf, after = %.2lf\n", nbits_input, nbits_output);
    printf("Compression ratio is %lf\n", nbits_input/nbits_output);

    fclose(output_file);
    fclose(input_file);
}

void compression(const char *outputname, const char *inputname)
{
    FILE *input_file = fopen(inputname, "r");

    data info[NB_MAX_CHAR] = {{0},{0},{0},{0}};
    short num_of_unique_char = put_char_freq_in_file(input_file, info); // Кол-во уникальных символов
    data only_char[num_of_unique_char];

    array_copy_only_data(info, only_char, num_of_unique_char);
    array_sort(only_char, num_of_unique_char);

    hqueue *priority_queue = NULL;
    priority_queue = fill_queue_with_tree(priority_queue, only_char, num_of_unique_char);

    htree *huffman_tree = NULL;
    huffman_tree = build_huffman_tree(priority_queue);

    put_huffman_code_in_info(huffman_tree, info, 0, 0);

    encoding(inputname, outputname, info, num_of_unique_char);

    tree_free(huffman_tree);
}