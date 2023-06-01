#include <protocol.h>
#include <stdlib.h>

size_t serialize_time(char *body, uint8_t hours, uint8_t minutes)
{
    uint8_t payload[2] = {hours, minutes};
    memcpy(body, payload, sizeof(payload));
    return sizeof(payload);
}

size_t serialize_char(char *body, uint8_t *value)
{
    memcpy(body, value, sizeof(uint8_t));
    return sizeof(uint8_t);
}

size_t deserialize_time(char *body, uint8_t *hours, uint8_t *minutes)
{
    uint8_t payload[2];
    memcpy(payload, body, sizeof(payload));
    *hours = payload[0];
    *minutes = payload[1];
    return sizeof(payload);
}

size_t deserialize_char(char *body, uint8_t *ret)
{
    memcpy(ret, body, sizeof(uint8_t));
    return sizeof(uint8_t);
}