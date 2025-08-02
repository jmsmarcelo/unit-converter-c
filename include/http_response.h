#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "../include/client_header.h"

void send_response(client_header *header, const char *body);
const char *get_status(int code);
void set_default_status_msg(client_header *header);

#endif