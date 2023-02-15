#ifndef FUNC_H
#define FUNC_H

#include "huffman.h"

void array_copy_only_data(data *array, data *cpy_array, short size);
void array_sort(data *array, short size);
hqueue *queue_push(hqueue *queue, htree *tree);
hqueue *queue_pop(hqueue *queue);
void queue_free(hqueue *queue);
htree *tree_create(unsigned char ch, unsigned freq);
htree *tree_create_parent(htree *tree1, htree *tree2);
void tree_free(htree *root);

#endif