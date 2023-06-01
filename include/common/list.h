#ifndef _LIST
#define _LIST
#include <string.h>

typedef struct _list list_t;

struct _list
{
    void * arr;
    size_t num;
    size_t size;
    size_t capacity;
};

list_t * create_list(size_t item_size);
void push_item(list_t *list, void * item);
void delete_item(list_t *list, size_t index);
void _double_list(list_t *list);
void *pop_item(list_t *list);
void *access_item(list_t *list, size_t index);

#endif
