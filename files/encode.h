#ifndef ENCODE_H
#define ENCODE_H

#include "huffman.h"

short get_char_freq_in_file(FILE *input_file, data *info);
hqueue *fill_queue_with_tree(hqueue *queue, data *array, short size);
htree *build_huffman_tree(hqueue *queue);
void get_huffman_code_in_info(htree *root, data *info, unsigned code, unsigned nbits);
void write_info_in_binary_file(FILE *output_file, data *info, short size);
void encoding(const char *inputname, const char *outputname, data *info, short size);
void compression(const char *outputname, const char *inputname);

#endif