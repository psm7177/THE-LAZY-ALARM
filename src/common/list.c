#include <list.h>
#include <stdlib.h>
#include <stdio.h>
#include <mission.h>

list_t *create_list(size_t item_size)
{
    list_t *list = malloc(sizeof(list_t));

    list->item_size = item_size;
    list->num = 0;
    list->capacity = 1;
    list->arr = malloc(list->item_size * list->capacity);

    return list;
}

void push_item(list_t *list, void *item)
{
    if (list->num == list->capacity)
    {
        _double_list(list);
    }
    char *p = list->arr;
    void *offset = p + list->num * list->item_size;
    memcpy(offset, item, list->item_size);

    list->num++;
}

void delete_item(list_t *list, size_t index)
{
    char *backup = list->arr;
    size_t size = list->item_size;

    if (list->num != 0)
    {
        void *p_delete = backup + index * size;
        memmove(p_delete, p_delete + size, size * (list->num - index - 1));
        list->num--;
    }
    else
    {
        fprintf(stderr, "There is no any of item in list\n");
        exit(1);
    }
}

void *access_item(list_t *list, size_t index)
{

    if (list->num == 0)
    {
        fprintf(stderr, "There is no any of item in list\n");
        exit(1);
    }

    if (index >= list->num)
    {
        fprintf(stderr, "Index error\n");
        exit(1);
    }
    else
        return list->arr + index * list->item_size;
}

void _double_list(list_t *list)
{
    size_t size = list->capacity * list->item_size;
    size_t double_size = 2 * size;

    char *aux = (char *)realloc(list->arr, double_size);

    if (list->arr == NULL)
    {
        fprintf(stderr, "Memory allocation is failed\n");
        exit(1);
    }

    list->arr = aux;
    list->capacity = 2 * list->capacity;
}

void *pop_item(list_t *list)
{
    void *last = list->arr + (list->num - 1) * list->item_size;
    delete_item(list, list->num - 1);
    return last;
};

int test_list()
{
    list_t *a = create_list(4);
    int b = 2;
    int *item = &b;
    push_item(a, item);
    push_item(a, item);
    push_item(a, item);

    int *result = access_item(a, 1);

    printf("%i\n", *result);
    return 0;
}