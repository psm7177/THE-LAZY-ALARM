#ifndef _PARSER
#define _PARSER
#include <stdint.h>
#include <stdbool.h>

#define COMMAND_CREATE 0

typedef struct _arg arg_t;
typedef struct _option option_t;
// alarm create hh:mm:ss [-d difficulty] [-m music] [-v volume] [-r the number of repeat]

struct _option
{
    uint8_t type;
    uint8_t value[2];
};

struct _arg
{
    uint8_t method;
    uint8_t hours;
    uint8_t minutes;
    uint8_t num_options;
    option_t options[5];
};

uint8_t parse_method(char *str);
void parse_create(arg_t *arg, int argc, char **argv);
void parse_time(char *str, uint8_t *hours, uint8_t *minutes);
void parse_arg(arg_t *arg, int argc, char **argv);
#endif