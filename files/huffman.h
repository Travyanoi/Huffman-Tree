#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define NB_MAX_CHAR 256

typedef struct data {
    unsigned char charact;  
    unsigned int freq;        	
	unsigned int hcode;
    unsigned int nbits;
} data;

typedef struct htree {
    unsigned char charact;
    unsigned int freq;
    unsigned int hcode;
    unsigned int nbits;
    struct htree *left;
    struct htree *right;
} htree_t, htree;

typedef struct hqueue {
    htree *tree;
    struct hqueue *next;
} hqueue_t, hqueue;

#endif