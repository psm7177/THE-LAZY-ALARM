#include <protocol.h>
#include <stdlib.h>
size_t create_request(request_t *req, uint8_t method, uint8_t id)
{
    memset(req, 0, REQUEST_SIZE);
    req->method = method;
    req->id = id;
    return 0;
}
void serialize_time(request_t *req, int hours, int minutes)
{
    int payload[] = {hours, minutes};
    memcpy(req->body, payload, sizeof(payload));
}

void deserialized_time(request_t *req, int *hours, int *minutes)
{
    int payload[2];
    memcpy(payload, req->body, sizeof(payload));
    *hours = payload[0];
    *minutes = payload[1];
}
// TODO: implement deserialize_time