#include <alarm.h>
#include <alarm-context.h>
#include <mission.h>
#include <music.h>
#include <stdint.h>
#include <unistd.h>
int START_ALARM = 0;
int STOP_ALARM = 1;

int32_t sig_idx;
pthread_mutex_t sig_mutex;
pthread_cond_t sig_cond;

alarm_t *create_alarm(uint8_t id, uint8_t hour, uint8_t minute)
{
    alarm_t *alarm = calloc(1, sizeof(alarm_t));

    alarm->id = id;
    alarm->target_time.tm_hour = hour;
    alarm->target_time.tm_min = minute;
    alarm->num_music = 0;
    // alarm->music = music;
    alarm->volume = VOLUME_MAX;
    // alarm->repeat_op = repeat_op;
    alarm->active = false;

    return alarm;
};

void delete_alarm(alarm_t *alarm)
{
    free(alarm);
}

// void turn_on(alarm_t *alarm) {
//     alarm->active = 1;
// }
// void turn_off(alarm_t *alarm) {
//     alarm->active = 0;
// }

void syncronize()
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("the time is %i : %i : %i\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    while (1)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        if (timeinfo->tm_sec != 0 && timeinfo->tm_sec != 1)
        {

            sleep(1);
        }
        else
            break;
    }
}

time_t time_check(list_t *alarm_list)
{
    time_t curr;
    struct tm *timeinfo;

    pthread_mutex_lock(&alarm_mutex);
    for (uint32_t i = 0; i < alarm_list->num; i++)
    {
        time(&curr);
        timeinfo = localtime(&curr);

        alarm_t *alarm = access_item(alarm_list, i);
        if (alarm->target_time.tm_hour == timeinfo->tm_hour && alarm->target_time.tm_min == timeinfo->tm_min)
        {
            pthread_mutex_lock(&sig_mutex);
            sig_idx = (int32_t)i;
            pthread_mutex_unlock(&sig_mutex);
            pthread_cond_signal(&sig_cond);
        }
    }
    pthread_mutex_unlock(&alarm_mutex);

    syncronize();
    return curr;
}

void *check_alarm()
{
    syncronize();
    while (1)
    {
        time_check(alarm_list);
        sleep(60);
    }
    return 0;
}

void *signal_handler()
{
    int32_t active_idx;
    while (1)
    {
        pthread_mutex_lock(&sig_mutex);
        while (sig_idx == -1)
        {
            pthread_cond_wait(&sig_cond, &sig_mutex);
        }
        active_idx = sig_idx;
        sig_idx = -1;
        pthread_mutex_unlock(&sig_mutex);

        pthread_mutex_lock(&alarm_mutex);
        alarm_t *alarm = access_item(alarm_list, active_idx);
        play_music(alarm->music);
        exe_mission(alarm->difficulty);
        // stop_music();
        pthread_mutex_unlock(&alarm_mutex);
    }
}

void init_alarm_sig()
{
    pthread_mutex_init(&sig_mutex, NULL);
    pthread_cond_init(&sig_cond, NULL);
    sig_idx = -1;
}

int test_()
{
    alarm_t *A = create_alarm(123, 23, 57);
    alarm_list = create_list(sizeof(alarm_t));

    push_item(alarm_list, A);

    pthread_t checking_alarm, handling;

    pthread_create(&checking_alarm, 0, &check_alarm, 0);
    pthread_create(&handling, 0, &signal_handler, 0);

    pthread_join(checking_alarm, 0);
    pthread_join(handling, 0);
    return 0;
}
