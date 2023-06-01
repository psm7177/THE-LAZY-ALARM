#define _XOPEN_SOURCE
#include <stdio.h>
#include <parser.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <protocol.h>

uint8_t parse_option(char *str);
uint8_t parse_int(char *str);
void parser_duplicated_option_exception(uint8_t option);

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
        uint8_t end;
        for (int j = 0; j < arg->num_options; j++)
        {
            if (option == arg->options[j].type)
            {
                parser_duplicated_option_exception(option);
            }
        }
        switch(option)
        {
            case OPTION_TIME:
                fprintf(stderr, "time option is not supported with create method.\n");
                exit(1);
                break;
            case OPTION_DIFFICULTY:
            case OPTION_MUSIC:
            case OPTION_REPEAT:
            case OPTION_VOLUME:
                end = arg->num_options;
                arg->options[end].type = option;
                *(arg->options[end].value) = parse_int(argv[i + 1]);
                arg->num_options++;
                break;
            default:
                fprintf(stderr,"Not Implemented %d\n",option);
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

uint8_t parse_int(char *str)
{
    char *end;
    uint8_t value = (uint8_t)strtol(str, &end, 10);
    if (end == str || *end != '\0')
    {
        fprintf(stderr, "%s is only inteager.\n", str);
        exit(1);
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

void parser_duplicated_option_exception(uint8_t option)
{
    char *error_option;
    switch (option)
    {
    case OPTION_DIFFICULTY:
                error_option = "difficulty";
                break;
            case OPTION_MUSIC:
                error_option = "music";
                break;
            case OPTION_REPEAT:
                error_option = "repeat";
                break;
            case OPTION_VOLUME:
                error_option = "volume";
                break;
            case OPTION_TIME:
                error_option = "time";
            default:
                break;
            }
            fprintf(stderr, "%s option are duplicated.\n", error_option);
            exit(1);
    }