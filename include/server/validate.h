#include <stdbool.h>
#include <protocol.h>

bool validate_request(request_t *req, response_t *res);
bool validate_method(uint8_t method);
