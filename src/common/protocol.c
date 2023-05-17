#include <protocol.h>
#include <stdlib.h>
size_t create_request(request_t *req,  uint8_t method, uint8_t id, uint8_t option)
{
    memset(req, 0,REQUEST_SIZE);
    req->method = method;
    req->id = id;
    req->option = option;
    return 0;
}
void serialize_time(request_t *req, char * str_time){
    // struct tm result;
    // if(strptime(str_time, "%H:%M",&result) == NULL){
    //     fprintf(stderr,"time ");
    //     exit(1);
    // }
    //TODO: implement serialziee_time and deserialize_time
}