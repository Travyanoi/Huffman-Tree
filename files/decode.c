#include "func.h"
#include "encode.h"
#include "decode.h"

short get_num_of_unique_char(FILE *input_file)
{
    short num_of_unique_char;
    fread(&num_of_unique_char, sizeof(short), 1, input_file);
    return num_of_unique_char;
}

void get_info_from_binary_file(FILE *input_file, data *info, short size)
{
    for (short i = 0; i < size; i++) {
        fread(&info[i].charact, sizeof(char), 1, input_file);
        fread(&info[i].freq, sizeof(unsigned), 1, input_file);
    }
}

void put_huffman_code_in_tree(htree *root, unsigned bin_code, unsigned nbits)
{ 
    if (!root->left && !root->right) {
        root->hcode = bin_code; root->nbits = nbits;
        return;
    }
    if (root->left){       
        put_huffman_code_in_tree(root->left, (bin_code << 1), nbits + 1);
    }
    if (root->right){
        put_huffman_code_in_tree(root->right, (bin_code << 1) + 1, nbits + 1);
    }
}

htree *search_character_in_tree(htree *tree, unsigned buffer, unsigned buffer_size, int nb_bits)
{
    if (nb_bits > buffer_size){
        return NULL;
    }

    if (!tree->left && !tree->right){
        return tree;
    }

    /* Рекурсивно движемся вправо, если последний бит после сдвига равен 0 */
    if (tree->left && ((buffer >> (buffer_size - nb_bits - 1)) & 1) == 0 )
        return search_character_in_tree(tree->left, buffer, buffer_size, nb_bits + 1);
    /* Рекурсивно движемся вправо, если последний бит после сдвига равен 1 */
    if (tree->right && ((buffer >> (buffer_size - nb_bits - 1)) & 1) == 1 ){
        return search_character_in_tree(tree->right, buffer, buffer_size, nb_bits + 1);
    }
    return NULL;
}

void decoding(FILE *input_file, FILE *output_file, htree *root)
{
    htree *decoded_node;
    unsigned char c_bin = 0, c_write = 0;
    unsigned int buffer = 0, buffer_size = 0, nb_total_char = 0;
    nb_total_char = root->freq;
    while (fread(&c_bin, sizeof(char), 1, input_file) > 0){
        buffer = (buffer << 8);
        buffer = (buffer | c_bin);
        buffer_size += 8;

        while ((decoded_node = search_character_in_tree(root, buffer, buffer_size, 0)) && nb_total_char){
            c_write = decoded_node->charact;
            fwrite(&c_write, sizeof(char), 1, output_file);
            nb_total_char--;
            buffer_size -= decoded_node->nbits;
        }
    }
    fclose(output_file);
    fclose(input_file);
}

void decompression(const char *outputname, const char *inputname)
{
    FILE *input_file = fopen(inputname, "rb");
    FILE *output_file = fopen(outputname, "w");

    short num_of_unique_char = get_num_of_unique_char(input_file); // Кол-во уникальных символов
    data info[num_of_unique_char];
    get_info_from_binary_file(input_file, info, num_of_unique_char);
    array_sort(info, num_of_unique_char);

    hqueue *priority_queue = NULL;
    priority_queue = fill_queue_with_tree(priority_queue, info, num_of_unique_char);

    htree *huffman_tree = NULL;
    huffman_tree = build_huffman_tree(priority_queue);

    put_huffman_code_in_tree(huffman_tree, 0, 0);

    decoding(input_file, output_file, huffman_tree);

    tree_free(huffman_tree);
}