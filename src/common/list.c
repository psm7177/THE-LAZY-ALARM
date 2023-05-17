#include <list.h>
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
    if (list->num == list->capacity)
    {
        _double_list(list);
    }
    char * p = list->arr;
    void *offset = p + list->num * list->size;
    memcpy(offset, item, list->size);

    list->num++;
}

// void delete_item(list_t *list, size_t index)
// {
    
// }

void _double_list(list_t *list)
{
    char *backup = list->arr;
    size_t size = list->num * list->capacity;
    size_t dubble_size = 2 * size;

    list->arr = (char *)realloc(list->arr, dubble_size);

    if (list->arr == NULL)
    {
        free(backup);
        fprintf(stderr, "Memory allocation is failed");
        exit(1);
    }

    if (backup != list->arr)
    {
        memcpy(list->arr, backup, size);
    }

    list->capacity = dubble_size;
}