#include <request.h>
#include <stdlib.h>

request_t *make_request(arg_t *arg)
{
    request_t *req = calloc(1, sizeof(request_t));

    char *cur = req->body;
    req->method = arg->method;
    switch (arg->method)
    {
    case CREATE:
        cur += serialize_time(cur, arg->hours, arg->minutes);
        break;
    case GET:
        cur += serialize_char(cur, &arg->id);
        break;
    case DELETE:
        cur += serialize_char(cur, &arg->id);
        break;
    case UPDATE:
        cur += serialize_char(cur, &arg->id);
        break;
    default:
        break;
    }

    for (int i = 0; i < arg->num_options; i++)
    {
        option_t option = arg->options[i];
        req->options[i] = option.type;
        switch (option.type)
        {
        case OPTION_ALL:
            cur += 1;
            break;
        case OPTION_MUSIC:
        case OPTION_DIFFICULTY:
        case OPTION_REPEAT:
        case OPTION_VOLUME:
            cur += serialize_char(cur, option.value);
            break;
        case OPTION_TIME:
            cur += serialize_time(cur, option.value[0], option.value[1]);
            break;
        default:
            break;
        }
    }
    req->num_options = arg->num_options;

    return req;
}