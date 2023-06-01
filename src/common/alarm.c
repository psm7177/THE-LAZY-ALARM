#include "alarm.h"
#include "mission.h"
#include "music.h"

alarm_t *create_alarm(uint8_t id, struct tm *target_time, list_t *mission_list, char *music, size_t volume, char *repeat_op) {
    alarm_t *alarm = malloc(sizeof(alarm_t));

    alarm->id = id;
    alarm->target_time = target_time;
    alarm->mission_list = mission_list;
    alarm->music = music;
    alarm->volume = volume;
    alarm->repeat_op = repeat_op;

    alarm->active = 0;
    return alarm;
};

void delete_alarm(alarm_t *alarm) {
    free(alarm);
}

void turn_on(alarm_t *alarm) {
    alarm->active = 1;
}
void turn_off(alarm_t *alarm) {
    alarm->active = 0;
}
void syncronize() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    while (1)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        if (timeinfo->tm_sec != 0 && timeinfo->tm_sec != 1) wait(1); else break;
    }
}

time_t time_check(list_t *alarm_list) {
    time_t curr;
    struct tm* timeinfo;
    time(&curr);
    timeinfo = localtime(&curr);

    for (size_t i = 0; i < alarm_list->num; i++)
    {
        alarm_t *alarm = access_item(alarm_list, i);
        if (alarm->target_time->tm_hour == timeinfo->tm_hour && alarm->target_time->tm_min == timeinfo->tm_min)
        {
            start_alarm(alarm);
        }
    }

    syncronize();
}

void start_alarm(alarm_t *alarm) {
    play_music(alarm->music);

    for (size_t i = 0; i < alarm->mission_list->num; i++)
    {
        mission_t *mission = (mission_t *) access_item(alarm->mission_list, i);
        exe_mission(mission);
    }
    
    turn_off_music();
}

int test() {
    time_t curr;
    struct tm* target_time = localtime(&curr);
    list_t *mission_list = create_list(sizeof(mission_t));
    alarm_t *a = create_alarm(123, target_time, mission_list, "music.wav", 10, "every monday");
    alarm_t *b = create_alarm(121, target_time, mission_list, "music.wav", 5, "every tuesday");

    list_t *alarm_list = create_list(sizeof(alarm_t));
    push_item(alarm_list, a);
    push_item(alarm_list, b);

    turn_on(a);
    turn_on(b);

    syncronize();
    
    while (1)
    {
        time_check(alarm_list);
        wait(60);
    }
}
