#ifndef CLIENT_HEADER_H
#define CLIENT_HEADER_H

#ifdef _WIN32
#include <WinSock2.h>
#endif

typedef struct {
#ifdef _WIN32
    SOCKET client_socket;
#else
    int client_socket;
#endif
    char *method;
    char *path;
    char *http_version;
    char *content_type;
    int content_length;
    int status_code;
    char *error_msg;
    char *body_request;
    char *body_response;
} client_header;

#endif