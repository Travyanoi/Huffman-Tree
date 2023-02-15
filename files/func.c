#include "func.h"

void array_copy_only_data(data *array, data *cpy_array, short size)
{
    for (short i = 0, j = 0; i < NB_MAX_CHAR && j < size; i++){
        if (array[i].freq == 0){
            continue;
        }
        cpy_array[j].charact = array[i].charact;
        cpy_array[j].freq = array[i].freq;
        j++;
    }
}

void array_sort(data *array, short size)
{
    short j;
    data tmp;
    for (int i = 1; i < size; i++){
        tmp = array[i];
        j = i - 1;
        while ((j >= 0) && (tmp.freq < array[j].freq)) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = tmp;
    }
}

hqueue *queue_push(hqueue *queue, htree *tree)
{
    hqueue *new = calloc(1, sizeof(hqueue_t));
    if (!new){
        printf("Memory hasn't allocate\n");
        exit(EXIT_FAILURE);
    }
    new->tree = tree; new->next = NULL; 
    if(!queue){
        return new;
    }
    hqueue *tmp = queue;
    while(tmp->next && tmp->next->tree->freq < new->tree->freq){
        tmp = tmp->next;
    }
    new->next = tmp->next;
    tmp->next = new;
    return queue;
}

hqueue *queue_pop(hqueue *queue)
{
    /* Удаляем первый элемент в очереди */
    if (!queue)
        return NULL;
    hqueue *tmp = queue->next;
    queue->next = NULL;
    free(queue);
    return tmp;
}

void queue_free(hqueue *queue)
{
    while (queue) {
        tree_free(queue->tree);
        queue = queue_pop(queue);
    }
}

htree *tree_create(unsigned char ch, unsigned freq)
{
    htree *new = calloc(1, sizeof(htree_t));
    if (!new) {
        printf("Hasn't allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }
    new->charact = ch;
    new->freq = freq;
    new->hcode = 0;
    new->nbits = 0;
    new->left = NULL;
    new->right = NULL;
    return new;
}

htree *tree_create_parent(htree *tree1, htree *tree2)
{
    htree *parent = tree_create('$', tree1->freq + tree2->freq);
    if (tree1->freq <= tree2->freq) {
        parent->left = tree1;
        parent->right = tree2;
    } else {
        parent->left = tree2;
        parent->right = tree1;
    }
    return parent;
}

void tree_free(htree *root)
{
    if (root){
        tree_free(root->left);
        tree_free(root->right);
        free(root);
        root = NULL;
    }
}