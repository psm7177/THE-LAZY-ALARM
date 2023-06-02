#include <alarm-context.h>
#include <alarm.h>

void init_alarm_list()
{
    alarm_list = create_list(sizeof(alarm_t));
    pthread_mutex_init(&alarm_mutex, NULL);
};

alarm_t *get_alarm_by_id(uint8_t id)
{
    for (uint32_t i = 0; i < alarm_list->num; i++)
    {
        alarm_t *p_alarm = access_item(alarm_list, i);
        if (p_alarm->id == id)
        {
            return p_alarm;
        }
    }
    return NULL;
}