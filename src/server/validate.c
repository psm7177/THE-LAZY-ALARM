#include <protocol.h>
#include <validate.h>
#include <response.h>

bool validate_request(request_t *req, response_t *res)
{
    if (!validate_method(req->method))
    {
        char msg[32];
        sprintf(msg, "%d is invalid method.", req->method);
        make_error_response(res, msg);

        return false;
    }
    return true;
}

bool validate_method(uint8_t method)
{
    switch (method)
    {
    case CREATE:
    case DELETE:
    case UPDATE:
    case GET:
        return true;
    default:
        return false;
    }
}