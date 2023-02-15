#include "func.h"
#include "encode.h"

short get_char_freq_in_file(FILE *input_file, data *info)
{

    unsigned char c = 0;
    while (fread(&c, sizeof(char), 1, input_file) > 0) {
        info[c].charact = c;
        info[c].freq++;
    }

    short nb_char = 0;
    for (short i = 0; i < NB_MAX_CHAR; i++) {
        if (info[i].freq == 0) continue;
        nb_char++;
    }
    fclose(input_file);
    return nb_char;
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

void get_huffman_code_in_info(htree *root, data *info, unsigned bin_code, unsigned nbits)
{
    if (!root->left && !root->right) {
        info[root->charact].hcode = bin_code;
        info[root->charact].nbits = nbits;
        return;
    }
    /* Сдвигаем код влево на единицу и прибавляем 0(ничего не делаем после сдвига) */
    if (root->left){     
        get_huffman_code_in_info(root->left, info, (bin_code << 1), nbits + 1);
    }
    /* Сдвигаем код влево на единицу и прибавляем 1 */
    if (root->right){
        get_huffman_code_in_info(root->right, info, (bin_code << 1) + 1, nbits + 1);
    }
}

void write_info_in_binary_file(FILE *output_file, data *info, short size)
{
    fwrite(&size, sizeof(short), 1, output_file);
    //printf("%d", size);
    for (short i = 0; i < NB_MAX_CHAR; i++){
        if (info[i].freq == 0) continue;
        fwrite(&info[i].charact, sizeof(char), 1, output_file);
        fwrite(&info[i].freq, sizeof(unsigned), 1, output_file);    
        //printf("charact = %c, freq = %d\n", info[i].charact, info[i].freq);
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
    //unsigned int out_in_terminal;
    while (fread(&read_char, sizeof(char), 1, input_file) > 0){
        buffer = (buffer << info[read_char].nbits);
        buffer = (buffer | info[read_char].hcode);
        buffer_size += info[read_char].nbits;

        while (buffer_size >= 8){
            
            buffer_size -= 8;
            c_bin = (buffer >> buffer_size);
            //out_in_terminal = c_bin;
            //printf("hex = %x ", out_in_terminal);
            //for (int i = 0; i < 8; i++){
                //printf("%c", (out_in_terminal & 0x80) ? '1' : '0');
                //out_in_terminal <<= 1;
            //}
            //printf("\n");
            fwrite(&c_bin, sizeof(char), 1, output_file);
            nbits_output += 8;
        }

        nbits_input += 8;
    }

    if (buffer_size && buffer_size < 8) {
        c_bin = (buffer << (8-buffer_size));
        //out_in_terminal = c_bin;
        //printf("hex = %x ", out_in_terminal);
        for (int i = 0; i < 8; i++){
            //printf("%c", (out_in_terminal & 0x80) ? '1' : '0');
            //out_in_terminal <<= 1;
        }
        //printf("\n");
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
    short nb_char = get_char_freq_in_file(input_file, info); // Кол-во уникальных символов
    data only_char[nb_char];

    array_copy_only_data(info, only_char, nb_char);
    array_sort(only_char, nb_char);

    hqueue *priority_queue = NULL;
    priority_queue = fill_queue_with_tree(priority_queue, only_char, nb_char);

    htree *huffman_tree = NULL;
    huffman_tree = build_huffman_tree(priority_queue);

    get_huffman_code_in_info(huffman_tree, info, 0, 0);

    encoding(inputname, outputname, info, nb_char);

    tree_free(huffman_tree);
}