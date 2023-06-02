#ifndef _RESPONSE
#define _RESPONSE
#include <protocol.h>
#include <stdbool.h>

void response(request_t* req, response_t *res);

void make_error_response(response_t *res, char *msg);
#endif