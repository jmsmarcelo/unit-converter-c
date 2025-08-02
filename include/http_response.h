#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "../include/client_header.h"

const char *get_status(int code);
void send_response(client_header *header, const char *body);
void set_default_status_msg(client_header *header);

#endif