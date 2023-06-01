#include <alarm-context.h>
#include <alarm.h>

void init_alarm_list(){
    alarm_list = create_list(sizeof(alarm_t));
    pthread_mutex_init(&alarm_mutex, NULL);
};