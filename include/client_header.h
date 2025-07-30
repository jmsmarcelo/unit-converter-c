#ifndef CLIENT_HEADER_H
#define CLIENT_HEADER_H

#ifdef _WIN32
#include <WinSock2.h>
#endif

typedef struct {
    int status_code;
    char method[32];
    char *path;
    char http_version[32];
    int content_length;
    char content_type[128];
    char error_msg;
    char *body_request;
    char *body_response;
#ifdef _WIN32
    SOCKET client_socket;
#else
    int client_socket;
#endif
} client_header;

#endif