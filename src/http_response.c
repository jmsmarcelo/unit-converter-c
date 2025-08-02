#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#endif
#include "../include/http_response.h"
#include "../include/server_utils.h"

int send_all(client_header *header, const char *data, int len) {
    int total_sent = 0;
    int sent;
    while(total_sent < len) {
        sent = send(header->client_socket, data + total_sent, len - total_sent, 0);
        if(sent <= 0) break;
        total_sent += sent;
    }
    return total_sent;
}
void send_response(client_header *header, const char *content) {
    header->content_length = strlen(content);
    int h_len = snprintf(NULL, 0,
                    "HTTP/1.1 %d %s\r\n"
                    "Content-Type: %s\r\n"
                    "Content-Length: %d\r\n"
                    "Connection: close\r\n\r\n",
                    header->status_code, get_status(header->status_code),
                    header->content_type,
                    header->content_length);
    char *h = malloc(h_len + 1);
    if(!h) return;
    snprintf(h, h_len + 1,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n",
        header->status_code, get_status(header->status_code),
        header->content_type,
        header->content_length);
    int sent_h = send_all(header, h, h_len);
    if(sent_h > 0) send_all(header, content, header->content_length);
    free(h);
}
const char *get_status(int code) {
    switch(code) {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        default: return "Internal Server Error";
    }
}
void set_default_status_msg(client_header *header) {
    switch(header->status_code) {
        case 400:
            set_string(&(header->status_msg), "Request body could not be read properly.");
            break;
        case 404:
            set_string(&(header->status_msg), "Page not found.");
            break;
        case 405:
            set_string(&(header->status_msg), "Method not allowed.");
            break;
        default:
            set_string(&(header->status_msg), "The server was unable to complete your request. Please try again later.");
    }
}