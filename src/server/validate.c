#include <protocol.h>
#include <validate.h>

bool validate_request(request_t *req)
{
    if (!validate_method(req->method))
    {
        fprintf(stderr, "%d is invalid method.", req->method);
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