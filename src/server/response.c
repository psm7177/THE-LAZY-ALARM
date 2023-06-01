#include <response.h>
#include <validate.h>
#include <alarm.h>
#include <pthread.h>
#include <alarm-context.h>

void response_create(request_t *req);

void response(request_t *req)
{
    validate_request(req);
    switch (req->method)
    {
    case CREATE:
        response_create(req);
        break;

    default:
        break;
    }
}

void response_create(request_t *req)
{
    uint8_t hour;
    uint8_t minute;

    char * current_body = req->body;
    current_body += deserialize_time(current_body,&hour, &minute);

    alarm_t * alarm = create_alarm(0, hour, minute);

    for(int i = 0; i < req->num_options; i++){
        switch(req->options[i]){
            case OPTION_DIFFICULTY:
                current_body += deserialize_char(current_body,&alarm->difficulty);
                break;
            case OPTION_MUSIC:
                current_body += deserialize_char(current_body, &alarm->num_music);
                break;
            case OPTION_REPEAT:
                current_body += deserialize_char(current_body, &alarm->num_repeat);
                break;
            case OPTION_VOLUME:
                current_body += deserialize_char(current_body, &alarm->volume);
                break;
            case OPTION_TIME:
                fprintf(stderr,"time option is not option of create method.\n");
                break;
        }
    }

    fprintf(stdout, "create alarm at %d:%d d:%d, music: %d, volume: %d, repeat: %d\n", hour, minute, alarm->difficulty, alarm->num_music, alarm->volume, alarm->num_repeat);

    pthread_mutex_lock(&alarm_mutex);
    push_item(alarm_list, alarm);
    pthread_mutex_unlock(&alarm_mutex);
}
