#include <pthread.h>
#include <list.h>

list_t* alarm_list;
pthread_mutex_t alarm_mutex;

void init_alarm_list();