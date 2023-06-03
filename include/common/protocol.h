#ifndef _PROTOCOL
#define _PROTOCOL
#define _XOPEN_SOURCE
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <alarm.h>

#define CREATE 0b00
#define GET 0b01
#define UPDATE 0b10
#define DELETE 0b11

#define OPTION_TIME 0b000
#define OPTION_DIFFICULTY 0b001
#define OPTION_MUSIC 0b010
#define OPTION_VOLUME 0b011
#define OPTION_REPEAT 0b100
#define OPTION_ALL 0b101

#define TYPE_ERROR 0b000
#define TYPE_ALARM 0b001
#define TYPE_MUSIC 0b011
#define TYPE_MSG 0b100

#define PAYLOAD_SIZE 1024
#define REQUEST_SIZE sizeof(request_t)

typedef struct _request request_t;
typedef struct _response response_t;

struct _request
{
    uint8_t method;
    uint8_t num_options;
    uint8_t options[5];
    char body[PAYLOAD_SIZE];
};

struct _response
{
    uint8_t type;
    uint8_t num_info;
    char body[PAYLOAD_SIZE];
};

size_t serialize_time(char *req_body, uint8_t hours, uint8_t minutes);
size_t serialize_char(char *req_body, uint8_t *value);
size_t deserialize_time(char *req_body, uint8_t *hours, uint8_t *minutes);
size_t deserialize_char(char *body, uint8_t *ret);

#endif