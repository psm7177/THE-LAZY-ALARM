#include "list.h"
#include <stdlib.h>
#include <stdio.h>

list_t *create_list(size_t item_size)
{
    list_t *list = malloc(sizeof(list_t));

    list->size = item_size;
    list->num = 0;
    list->capacity = 1;
    list->arr = malloc(list->size * list->capacity);

    return list;
}

void push_item(list_t *list, void *item)
{
    if (list->num == list->capacity) _double_list(list);
    char * p = list->arr;
    void *offset = p + list->num * list->size;
    memcpy(offset, item, list->size);

    list->num++;
}

void delete_item(list_t *list, size_t index)
{
    char *backup = list->arr;

    if (list->num != 0) {
        void *backward = backup + (index + 1) * list->size;
        void *forward = backup + index * list->size;
        memcpy(forward, backward, list->size * (list->num - index - 1));
        list->num--;
    } else {
        fprintf(stderr, "There is no any of item in list\n");
        exit(1);
    }
}

void *access_item(list_t *list, size_t index) {

    if (list->num == 0)
    {
        fprintf(stderr, "There is no any of item in list\n");
        exit(1);
    }
    
    if (index > list->num - 1 || index < 0)
    {
        fprintf(stderr, "Index error\n");
        exit(1);
    } else return list->arr + index * list->size;
    
}

void _double_list(list_t *list)
{
    char *backup = list->arr;
    size_t size = list->num * list->capacity;
    size_t double_size = 2 * size;

    list->arr = (char *)realloc(list->arr, double_size);

    if (list->arr == NULL)
    {
        free(backup);
        fprintf(stderr, "Memory allocation is failed\n");
        exit(1);
    }

    if (backup != list->arr) memcpy(list->arr, backup, size);

    list->capacity = double_size;
}

void *pop_item(list_t *list) {
    void *last = list->arr + (list->num - 1) * list->size;
    delete_item(list, list->num - 1);
    return last;
};

int main() {
    list_t *a = create_list(4);
    int b = 2;
    int *item = &b;
    push_item(a, item);
    push_item(a, item);
    push_item(a, item);

    int *result = access_item(a, 1);

    printf("%i\n", *result);
}
