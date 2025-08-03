#ifndef SERVER_SETUP_H
#define SERVER_SETUP_H

#ifdef _WIN32
#include <WinSock2.h>
#endif

const char *get_server_setup_error_msg(int code);
int get_http_port();
#ifdef _WIN32
SOCKET get_http_socket();
int is_valid_socket(SOCKET sock);
#else
int get_http_socket();
int is_valid_socket(int sock);
#endif
int setup_socket();
void close_http_socket();

#endif