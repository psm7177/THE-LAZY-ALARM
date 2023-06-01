#define _XOPEN_SOURCE
#include <stdio.h>
#include <parser.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <protocol.h>

uint8_t parse_option(char *str);
uint8_t parse_int(char* str);

void parse_arg(arg_t *arg, int argc, char **argv)
{
    memset(arg, 0, sizeof(arg_t));
    arg->method = parse_method(argv[1]);
    switch (arg->method)
    {
    case COMMAND_CREATE:
        parse_create(arg, argc, argv);
        break;
    default:
        break;
    }
}

void parse_create(arg_t *arg, int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "create option need <time>\n");
        exit(1);
    }
    parse_time(argv[2], &arg->hours, &arg->minutes);
    for (int i = 3; i < argc; i += 2)
    {
        uint8_t option = parse_option(argv[i]);
        switch (option)
        {
        case OPTION_DIFFICULTY:
            if(arg->is_difficulty) {
                fprintf(stderr, "difficulty option are duplicated.\n");
                exit(1);
            }
            arg->is_difficulty = true;
            arg->difficulty = parse_int(argv[i + 1]);
            break;
        case OPTION_MUSIC:
            if(arg->is_music){
                fprintf(stderr,"music options are duplicated.\n");
                exit(1);
            }
            arg->is_music = true;
            arg->music = parse_int(argv[i + 1]);
            break;
        case OPTION_REPEAT:
            if(arg->is_repeat){
                fprintf(stderr, "repeat options are duplicated.\n");
                exit(1);
            }
            arg->is_repeat = true;
            arg->repeat = parse_int(argv[i + 1]);
            break;
        case OPTION_VOLUME:
            if(arg->is_volume){
                fprintf(stderr, "volume options are duplicated.\n");
            }
            arg->is_volume = true;
            arg->volume = parse_int(argv[i + 1]);
            break;
        case OPTION_TIME:
            fprintf(stderr, "time option is not supported with create method.\n");
            exit(1);
        default:
            break;
        }
    }
}

uint8_t parse_method(char *str)
{
    if (strcmp(str, "create") == 0)
    {
        return COMMAND_CREATE;
    }
    else
    {
        fprintf(stderr, "%s is not supported command.\n", str);
        exit(1);
    }
}

uint8_t parse_option(char *str)
{
    if (strcmp(str, "-d") == 0)
    {
        return OPTION_DIFFICULTY;
    }
    else if (strcmp(str, "-m"))
    {
        return OPTION_MUSIC;
    }
    else if (strcmp(str, "-v"))
    {
        return OPTION_VOLUME;
    }
    else if (strcmp(str, "-r"))
    {
        return OPTION_REPEAT;
    }
    else if (strcmp(str, "-t"))
    {
        return OPTION_TIME;
    }
    else
    {
        fprintf(stderr, "%s is not supported option.\n", str);
        exit(1);
    }
}

uint8_t parse_int(char* str)
{
    char * end;
    uint8_t value = (uint8_t)strtol(str, &end, 10); 
    if (end == str || *end != '\0'){
        fprintf(stderr, "%s is only inteager.\n", str);
    }
    return value;
}

void parse_time(char *str, uint8_t *hours, uint8_t *minutes)
{
    struct tm result;
    if (strptime(str, "%H:%M", &result) == 0)
    {
        fprintf(stderr, "<time> must be HH:MM.\n");
        exit(1);
    }
    *hours = result.tm_hour;
    *minutes = result.tm_min;
}