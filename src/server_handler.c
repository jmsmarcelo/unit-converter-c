#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif
#include "../include/server_handler.h"
#include "../include/client_header.h"
#include "../include/server_utils.h"
#include "../include/api_handler.h"
#include "../include/resource_handler.h"

#define BUFFER_SIZE 4096

int extract_body(const char *rest, client_header *header) {
    char buffer[BUFFER_SIZE];
    int bytes_read = 0;
    rest += 2;
    size_t total_read = strlen(rest);
    size_t body_cap = total_read + 1;
    header->body_request = malloc(body_cap);
    if(!header->body_request) return 1;
    memcpy(header->body_request, rest, total_read);
    while(total_read < header->content_length) {
        bytes_read = recv(header->client_socket, buffer, BUFFER_SIZE, 0);
        if(bytes_read < 0) {
            header->status_code = 0;
            return 1;
        } else if(bytes_read == 0) {
            break;
        }
        body_cap += bytes_read;
        char *ptr = realloc(header->body_request, body_cap);
        if(!ptr) {
            header->status_code = 500;
            return 1;
        }
        header->body_request = ptr;
        memcpy(header->body_request + total_read, buffer, bytes_read);
        total_read += bytes_read;
    }
    (header->body_request)[total_read] = '\0';
    return 0;
}
const char *extract_headers(const char *rest, client_header *header) {
    int status = 0;
    size_t key_cap = 32;
    char *key = malloc(key_cap);
    size_t value_cap = 32;
    char *value = malloc(value_cap);
    if(!key || !value) {
        status = 1;
        header->status_code = 500;
        goto cleanup;
    }
    while(rest && *rest != '\0' && strncmp(rest, "\r\n", 2) != 0) {
        rest = extract_until(extract_until(rest, ": ", &key, &key_cap), "\r\n", &value, &value_cap);
        if(!rest) break;
        if(strcmp(key, "Content-Length") == 0) {
            header->content_length = atoi(value);
        } else if(strcmp(key, "Content-Type") == 0) {
            set_string(&(header->content_type), value);
        }
    }
cleanup:
    if(key) free(key);
    if(value) free(value);
    if(status == 0) {
        return rest;
    }
    return NULL;
}
const char *extract_http_info(const char *request, client_header *header) {
    return extract_until(
        extract_until(extract_until(request, " ", &(header->method), NULL), " ", &(header->path), NULL), "\r\n", &(header->http_version), NULL
    );
}
int read_request(char **request, client_header *header) {
    int request_capacity = BUFFER_SIZE + 1;
    *request = malloc(request_capacity);
    if(!*request) {
        header->status_code = 500;
        return 1;
    }
    int request_len = 0;
    char buffer[BUFFER_SIZE];
    int bytes_read = 0;
    while(1) {
        bytes_read = recv(header->client_socket, buffer, BUFFER_SIZE, 0);
        if(bytes_read < 0) {
            header->status_code = 0;
            return 1;
        } else if(bytes_read == 0) {
            break;
        }
        memcpy(*request + request_len, buffer, bytes_read);
        request_len += bytes_read;
        if(strstr(*request, "\r\n\r\n")) {
            break;
        } else {
            request_capacity += BUFFER_SIZE;
            char *temp = realloc(*request, request_capacity);
            if(!temp) {
                header->status_code = 500;
                return 1;
            }
            *request = temp;
        }
    }
    (*request)[request_len] = '\0';
    return 0;
}
void handle_request(client_header *header) {
    char *request;
    if(read_request(&request, header) > 0) goto cleanup;
    const char *rest = extract_http_info(request, header);
    if(!rest) {
        header->status_code = 0;
        goto cleanup;
    }
    rest = extract_headers(rest, header);
    if(!rest) {
        header->status_code = 500;
        goto cleanup;
    }
    if(header->content_length > 0) {
        if(extract_body(rest, header) > 0) {
            header->status_code = 500;
            goto cleanup;
        }
    }
    header->status_code = 200;
cleanup:
    if(request) free(request);
}
#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID arg) {
#else
void *handle_client(void *arg) {
#endif
    client_header *header = (client_header*)arg;
    header->method = NULL;
    header->path = NULL;
    header->http_version = NULL;
    header->content_length = 0;
    header->content_type = NULL;
    header->error_msg = NULL;
    header->body_request = NULL;
    header->body_response = NULL;
    handle_request(header);
    if(header->status_code == 0 || !header->path) {
        fprintf(stderr, "[WARN] Invalid request or client disconnected.\n");
    } else if(strncmp(header->path, "/api/", 5) == 0) {
        handle_api(header);
    } else {
        handle_resource(header);
    }
    if(header->method) free(header->method);
    if(header->path) free(header->path);
    if(header->http_version) free(header->http_version);
    if(header->error_msg) free(header->error_msg);
    if(header->body_request) free(header->body_request);
    if(header->body_response) free(header->body_response);
#ifdef _WIN32
    shutdown(header->client_socket, SD_BOTH);
    closesocket(header->client_socket);
    free(header);
    return 0;
#else
    shutdown(header->client_socket, SHUT_RDWR);
    close(header->client_socket);
    free(header);
    return NULL;
#endif
}