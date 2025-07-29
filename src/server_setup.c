#include <stdio.h>
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif
#include "../include/server_setup.h"

static const char *error_msgs[] = {
    "OK.",
    "Error initializing WinSock.",
    "Error creating HTTP server socket.",
    "Error associating HTTP socket with port.",
    "Error listing HTTP for connections."
};
size_t error_msgs_count = (sizeof(error_msgs) / sizeof(error_msgs[0]));
const char *get_server_setup_error_msg(int code) {
    if(code < 0 || code >= error_msgs_count) return "Unknown error.";
    return error_msgs[code];
}
static int http_port = 8080;
void init_http_port() {
    char *env = getenv("HTTP_PORT");
    if(env != NULL) {
        int port = atoi(env);
        if(port > 0 && port <= 65535) http_port = port;
    }
}
int get_http_port() {
    return http_port;
}
#ifdef _WIN32
WSADATA wsa_data;
int wsa_initialized = 0;
char opt = 1;
SOCKET invalid_socket = INVALID_SOCKET;
SOCKET http_socket = INVALID_SOCKET;
SOCKET get_http_socket() {
    return http_socket;
}
int is_valid_socket(SOCKET sock) {
#else
int opt = 1;
int invalid_socket = -1;
int http_socket = -1;
int get_http_socket() {
    return http_socket;
}
int is_valid_socket(int sock) {
#endif
    return sock != invalid_socket;
}
int setup_socket() {
    init_http_port();
#ifdef _WIN32
    if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        return 1;
    }
    wsa_initialized = 1;
#endif
    http_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(http_socket == invalid_socket) {
        return 2;
    }
    setsockopt(http_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = INADDR_ANY;
    socket_addr.sin_port = htons(http_port);
    if(bind(http_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) < 0) {
        return 3;
    }
    if(listen(http_socket, 10) < 0) {
        return 4;
    }
    return 0;
}
void close_http_socket() {
#ifdef _WIN32
    if(http_socket != invalid_socket) closesocket(http_socket);
    if(wsa_initialized) WSACleanup();
#else
    if(http_socket != invalid_socket) close(http_socket);
#endif
    http_socket = invalid_socket;
}