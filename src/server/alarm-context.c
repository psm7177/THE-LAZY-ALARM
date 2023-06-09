#include <alarm-context.h>
#include <alarm.h>

void init_alarm_list()
{
    alarm_list = create_list(sizeof(alarm_t));
    pthread_mutex_init(&alarm_mutex, NULL);
};

alarm_t *get_alarm_by_id(uint8_t id)
{
    int32_t idx = get_alarm_index_by_id(id);
    if (idx == -1)
    {
        return NULL;
    }
    return access_item(alarm_list, idx);
}

int32_t get_alarm_index_by_id(uint8_t id)
{
    for (uint32_t i = 0; i < alarm_list->num; i++)
    {
        alarm_t *p_alarm = access_item(alarm_list, i);
        if (p_alarm->id == id)
        {
            return i;
        }
    }
    return -1;
}

void clean_alarm_list()
{
    alarm_list->num = 0;
}