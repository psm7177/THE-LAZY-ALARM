#include <alarm.h>
#include <alarm-context.h>
#include <mission.h>
#include <music.h>
#include <stdint.h>
#include <unistd.h>
int START_ALARM = 0;
int STOP_ALARM = 1;

typedef struct _alarm_sig {
    list_t* sig_list;
    list_t* sig_alarm_list;


    pthread_mutex_t sig_mutex;
    pthread_cond_t sig_cond;

}alarm_sig_t;

alarm_sig_t alarm_sig_set;

alarm_t *create_alarm(uint8_t id, uint8_t hour, uint8_t minute) {
    alarm_t *alarm = malloc(sizeof(alarm_t));

    alarm->id = id;
    alarm->target_time.tm_hour = hour;
    alarm->target_time.tm_min = minute;
    alarm->mission_list = create_list(sizeof(mission_t));
    alarm->num_music = 0;
    // alarm->music = music;
    alarm->volume = VOLUME_MAX;
    // alarm->repeat_op = repeat_op;
    // alarm->active = 0;

    return alarm;
};

void delete_alarm(alarm_t *alarm) {
    free(alarm);
}

// void turn_on(alarm_t *alarm) {
//     alarm->active = 1;
// }
// void turn_off(alarm_t *alarm) {
//     alarm->active = 0;
// }

void syncronize() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("the time is %i : %i : %i\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    while (1)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        if (timeinfo->tm_sec != 0 && timeinfo->tm_sec != 1) sleep(1); else break;
    }
}

time_t time_check(list_t *alarm_list) {
    time_t curr;
    struct tm* timeinfo;

    pthread_mutex_lock(&alarm_mutex);
    for (size_t i = 0; i < alarm_list->num; i++)
    {
        time(&curr);
        timeinfo = localtime(&curr);

        alarm_t *alarm = access_item(alarm_list, i);
        if (alarm->target_time.tm_hour == timeinfo->tm_hour && alarm->target_time.tm_min == timeinfo->tm_min)
        {
            pthread_mutex_lock(&alarm_sig_set.sig_mutex);

            push_item(alarm_sig_set.sig_list, &START_ALARM);
            push_item(alarm_sig_set.sig_alarm_list, alarm);
            pthread_cond_signal(&alarm_sig_set.sig_cond);

            pthread_mutex_unlock(&alarm_sig_set.sig_mutex);
        }
    }
    pthread_mutex_unlock(&alarm_mutex);
    
    syncronize();
    return curr;
}

void *check_alarm() {
    syncronize();
    while (1)
    {
        time_check(alarm_list);
        sleep(60);
    }
    return 0;
}

void *signal_handler(){
    while (1)
    {
        pthread_mutex_lock(&alarm_sig_set.sig_mutex);
        if (alarm_sig_set.sig_list->num == 0)
        {
            pthread_cond_signal(&alarm_sig_set.sig_cond);
        } else {
            int* single_sign = access_item(alarm_sig_set.sig_list, 0);
            alarm_t* target = access_item(alarm_sig_set.sig_alarm_list, 0);
            switch (*single_sign)
            {
            case 0: //알람 켜기 + 미션주기
                play_music(target->music);

                for (size_t i = 0; i < target->mission_list->num; i++)
                {
                    mission_t *mission = (mission_t *) access_item(target->mission_list, i);
                    exe_mission(mission);
                }
                delete_item(alarm_sig_set.sig_list, 0);
                delete_item(alarm_sig_set.sig_alarm_list, 0);
                push_item(alarm_sig_set.sig_list, &STOP_ALARM);
                push_item(alarm_sig_set.sig_alarm_list, target);
                pthread_cond_signal(&alarm_sig_set.sig_cond);
                break;
            case 1: //알람 끄기
                turn_off_music();
                delete_item(alarm_sig_set.sig_list, 0);
                delete_item(alarm_sig_set.sig_alarm_list, 0);
                break;
                    
            //TODO : implement other cases ex) setting alarm

            default:
                break;
            }
        }
        pthread_mutex_unlock(&alarm_sig_set.sig_mutex);
    }
    return 0;
}

int test_() {
    alarm_t *A = create_alarm(123, 23, 57);
    A->mission_list = create_list(sizeof(char*));
    alarm_list = create_list(sizeof(alarm_t));
    alarm_sig_set.sig_alarm_list = create_list(sizeof(alarm_t));
    alarm_sig_set.sig_list = create_list(sizeof(int));
    mission_t* mission;
    mission->name = "press buttons";
    mission->difficulty = 1;
    mission->success = 0;
    push_item(A->mission_list, mission);
    push_item(alarm_list, A);

    pthread_t checking_alarm, handling;

    pthread_create(&checking_alarm, 0, &check_alarm, 0);
    pthread_create(&handling, 0, &signal_handler, 0);

    pthread_join(checking_alarm, 0);
    pthread_join(handling, 0);
    return 0;
}
