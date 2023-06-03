#ifndef _ALARM_CONTEXT
#define _ALARM_CONTEXT

#include <pthread.h>
#include <list.h>
#include <alarm.h>

list_t *alarm_list;
pthread_mutex_t alarm_mutex;

void init_alarm_list();
alarm_t *get_alarm_by_id(uint8_t id);
int32_t get_alarm_index_by_id(uint8_t id);
void clean_alarm_list();
#endif