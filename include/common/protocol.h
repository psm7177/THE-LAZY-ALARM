#define _XOPEN_SOURCE
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#define CREATE 0b00
#define GET 0b01
#define UPDATE 0b10
#define DELETE 0b11

#define OPTION_TIME 0b000
#define OPTION_DIFFICULTY 0b001
#define OPTION_MUSIC 0b010
#define OPTION_VOLUME 0b011
#define OPTION_REPEAT 0b100

#define PAYLOAD_SIZE 64
#define REQUEST_SIZE sizeof(request_t)

typedef struct _request request_t;

struct _request{
    uint8_t method;
    uint8_t id;
    uint8_t option;
    char body[PAYLOAD_SIZE];
};

size_t create_request(request_t *req,  uint8_t method, uint8_t id, uint8_t option);

void serialize_time(request_t *req, char * str_time);
void deserialize_time(request_t *req, char * str_time);
