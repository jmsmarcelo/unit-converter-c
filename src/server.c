#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#endif
#include "../include/server.h"
#include "../include/server_setup.h"
#include "../include/client_header.h"
#include "../include/server_handler.h"

#ifdef _WIN32
DWORD socket_timeout = 20000;
#else
struct timeval socket_timeout = {.tv_sec = 20, .tv_usec = 0};
#endif
int running = 1;
void handle_sigint(int sig) {
    printf("\n[INFO] Safely shutting down the server in at 30 seconds...\n");
    running = 0;
}
int start_server() {
    signal(SIGINT, handle_sigint);
    int status = setup_socket();
    if(status != 0) {
        fprintf(stderr, "[ERROR] %s\n", get_server_setup_error_msg(status));
        return status;
    } else {
        printf("[INFO] HTTP Server started on port %d\n", get_http_port());
    }
    while(running) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(get_http_socket(), &fds);
        struct timeval select_timeout = {.tv_sec = 30, .tv_usec = 0};
        int  activity = select(get_http_socket() + 1, &fds, NULL, NULL, &select_timeout);
        if(activity > 0 && FD_ISSET(get_http_socket(), &fds)) {
            client_header *header = malloc(sizeof(client_header));
            header->client_socket = accept(get_http_socket(), NULL, NULL);
            if(!is_valid_socket(header->client_socket)) {
                perror("Error accepting connection");
                free(header);
                continue;
            }
#ifdef _WIN32
            setsockopt(header->client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&socket_timeout, sizeof(socket_timeout));
            setsockopt(header->client_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&socket_timeout, sizeof(socket_timeout));
            HANDLE thread_handle = CreateThread(NULL, 0, handle_client, header, 0, NULL);
            if(thread_handle != NULL) {
                CloseHandle(thread_handle);
            } else {
                perror("Error creating thread");
                shutdown(header->client_socket, SD_BOTH);
                closesocket(header->client_socket);
                free(header);
            }
#else
            setsockopt(header->client_socket, SOL_SOCKET, SO_RCVTIMEO, &socket_timeout, sizeof(socket_timeout));
            setsockopt(header->client_socket, SOL_SOCKET, SO_SNDTIMEO, &socket_timeout, sizeof(socket_timeout));
            pthread_t thread_id;
            if(pthread_create(&thread_id, NULL, handle_client, header) != 0) {
                perror("Error creating thread");
                shutdown(header->client_socket, SHUT_RDWR);
                close(header->client_socket);
                free(header);
            } else {
                pthread_detach(thread_id);
            }
#endif
        }
    }
    close_http_socket();
    return 0;
}