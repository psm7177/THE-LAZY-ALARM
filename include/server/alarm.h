#ifndef _ALARM
#define _ALARM
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <list.h>
#include <stdint.h>

#define VOLUME_MAX 100
//음악재생, 음악 리스트, 음악 고르기, 음악 몇 회 재생

//반복 (매일, 월요일마다, 화요일마다 ...)
//알람 이름
//재생할 음악
//음악 볼륨
//알람 미션 리스트
//알람 생성
// 알람 삭제

typedef struct _alarm alarm_t;

struct _alarm
{
    uint8_t id;
    struct tm target_time;
    list_t *mission_list;
    uint8_t difficulty;
    uint8_t num_repeat;
    uint8_t volume;
    size_t active;
    uint8_t num_music;
    char *music;
    char *repeat_op;
};

alarm_t *create_alarm(uint8_t id, uint8_t hour, uint8_t minute);
void delete_alarm(alarm_t *alarm);

void turn_on(alarm_t *alarm);
void turn_off(alarm_t *alarm);
void syncronize();

time_t time_check(list_t *alarm_list);

void start_alarm(alarm_t *alarm);
#endif