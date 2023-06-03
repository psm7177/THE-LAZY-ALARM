#include <response.h>
#include <validate.h>
#include <alarm.h>
#include <pthread.h>
#include <alarm-context.h>
#include <protocol.h>

void response_create(request_t *req, response_t *res);
void response_get(request_t *req, response_t *res);

void response_get_all(response_t *res);
void response_get_alarm(response_t *res, alarm_t *p_alarm);

void response_delete(request_t *req, response_t *res);

char *serialize_alarm(char *cur, alarm_t *p_alarm);

void response(request_t *req, response_t *res)
{
    validate_request(req, res);
    switch (req->method)
    {
    case CREATE:
        response_create(req, res);
        break;
    case GET:
        response_get(req, res);
        break;
    case DELETE:
        response_delete(req, res);
        break;
    default:
        break;
    }
}

void response_create(request_t *req, response_t *res)
{
    uint8_t hour;
    uint8_t minute;

    char *current_body = req->body;
    current_body += deserialize_time(current_body, &hour, &minute);

    alarm_t *alarm = create_alarm(0, hour, minute);

    for (int i = 0; i < req->num_options; i++)
    {
        switch (req->options[i])
        {
        case OPTION_DIFFICULTY:
            current_body += deserialize_char(current_body, &alarm->difficulty);
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
            make_error_response(res, "time option is not option of create method.");
            break;
        }
    }

    fprintf(stdout, "create alarm at %d:%d d:%d, music: %d, volume: %d, repeat: %d\n", hour, minute, alarm->difficulty, alarm->num_music, alarm->volume, alarm->num_repeat);

    response_get_alarm(res, alarm);

    pthread_mutex_lock(&alarm_mutex);
    push_item(alarm_list, alarm);
    pthread_mutex_unlock(&alarm_mutex);
    free(alarm);
}

void response_get(request_t *req, response_t *res)
{
    uint8_t id;
    char *current_body = req->body;
    current_body += deserialize_char(current_body, &id);

    for (int i = 0; i < req->num_options; i++)
    {
        switch (req->options[i])
        {
        case OPTION_DIFFICULTY:
        case OPTION_REPEAT:
        case OPTION_VOLUME:
        case OPTION_TIME:
            make_error_response(res, "%d option is not option of get method.");
            break;
        case OPTION_ALL:
            response_get_all(res);
            return;
        case OPTION_MUSIC:
            make_error_response(res, "Not Implemented response_get_music");
            return;
        }
    }
    if (id == 255)
    {
        make_error_response(res, "invalid access.");
        return;
    }
    alarm_t *p_alarm = get_alarm_by_id(id);
    if (p_alarm == NULL)
    {
        make_error_response(res, "It does not exist.");
        return;
    }
    response_get_alarm(res, p_alarm);
}

void response_get_all(response_t *res)
{
    res->type = TYPE_ALARM;
    char *cur = res->body;

    pthread_mutex_lock(&alarm_mutex);
    res->num_info = alarm_list->num;

    for (uint32_t i = 0; i < alarm_list->num; i++)
    {
        alarm_t *p_alarm = access_item(alarm_list, i);
        cur = serialize_alarm(cur, p_alarm);
    }
    pthread_mutex_unlock(&alarm_mutex);
}

void response_get_alarm(response_t *res, alarm_t *p_alarm)
{
    res->type = TYPE_ALARM;
    res->num_info = 1;
    char *cur = res->body;

    pthread_mutex_lock(&alarm_mutex);
    serialize_alarm(cur, p_alarm);
    pthread_mutex_unlock(&alarm_mutex);
}

void response_delete(request_t *req, response_t *res)
{
    uint8_t id;
    char *current_body = req->body;
    current_body += deserialize_char(current_body, &id);

    char msg[64];
    for (int i = 0; i < req->num_options; i++)
    {
        switch (req->options[i])
        {
        case OPTION_DIFFICULTY:
        case OPTION_REPEAT:
        case OPTION_VOLUME:
        case OPTION_TIME:
            sprintf(msg, "%d option is not option of get method.", req->options[i]);
            make_error_response(res, msg);
            break;
        case OPTION_ALL:
            pthread_mutex_lock(&alarm_mutex);
            clean_alarm_list();
            pthread_mutex_unlock(&alarm_mutex);
            res->type = TYPE_MSG;
            res->num_info = 0;
            sprintf(res->body, "SUCCESS");
            return;
        case OPTION_MUSIC:
            make_error_response(res, "Not Implemented response_get_music");
            return;
        }
    }
    if (id == 255)
    {
        make_error_response(res, "Invalid access.");
        return;
    }
    pthread_mutex_lock(&alarm_mutex);
    int32_t idx = get_alarm_index_by_id(id);
    if (idx == -1)
    {
        make_error_response(res, "Alarm not found for the given ID.");
        pthread_mutex_unlock(&alarm_mutex);
        return;
    }

    alarm_t *p_alarm = access_item(alarm_list, idx);
    if (p_alarm == NULL)
    {
        fprintf(stderr, "ASSERT p_alarm != NULL\n");
        exit(1);
    }

    delete_item(alarm_list, idx);
    pthread_mutex_unlock(&alarm_mutex);

    res->type = TYPE_MSG;
    res->num_info = 0;
    sprintf(res->body, "SUCCESS");
}

char *serialize_alarm(char *cur, alarm_t *p_alarm)
{
    uint8_t hour;
    uint8_t minute;
    hour = p_alarm->target_time.tm_hour;
    minute = p_alarm->target_time.tm_min;

    cur += serialize_char(cur, &p_alarm->id);
    cur += serialize_time(cur, hour, minute);
    cur += serialize_char(cur, &p_alarm->difficulty);
    cur += serialize_char(cur, &p_alarm->num_music);
    cur += serialize_char(cur, &p_alarm->volume);
    cur += serialize_char(cur, &p_alarm->num_repeat);

    return cur;
}

void make_error_response(response_t *res, char *msg)
{
    res->type = TYPE_ERROR;
    strcpy(res->body, msg);
    fprintf(stderr, "%s\n", msg);
}