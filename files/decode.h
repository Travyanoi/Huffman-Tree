#ifndef DECODE_H
#define DECODE_H

#include "huffman.h"

short get_nb_char(FILE *input_file);
void get_info_from_binary_file(FILE *input_file, data *hdico, short size);
void put_huffman_code_in_tree(htree *root, unsigned bin_code, unsigned nbits);
htree *search_character_in_tree(htree *tree, unsigned buffer, unsigned buffer_size, int nb_bits);
void decoding(FILE *input_file, FILE *output_file, htree *root);
void decompression(const char *outputname, const char *inputname);

#endif