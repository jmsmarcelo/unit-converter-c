#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../include/server_utils.h"

const char *extract_until(const char *src, const char *delim, char **out, size_t *out_cap) {
    if(!src) return NULL;
    const char *pos = strstr(src, delim);
    if(!pos) return NULL;
    size_t out_len = pos - src;
    if(!out_cap || *out_cap <= out_len) {
        if(out_cap) *out_cap = out_len + 1;
        char *temp = *out ? realloc(*out, out_len + 1) : malloc(out_len + 1);
        if(!temp) return NULL;
        *out = temp;
    }
    memcpy(*out, src, out_len);
    (*out)[out_len] = '\0';
    return pos + strlen(delim);
}
int set_string(char **field, const char *value) {
    if(!field || !value) return 0;
    int value_len = strlen(value);
    char *ptr = *field ? realloc(*field, value_len + 1) : malloc(value_len + 1);
    if(!ptr) return 0;
    *field = ptr;
    strcpy(*field, value);
    return value_len;
}
int set_formatted_string(char **field, const char *fmt, ...) {
    va_list args, args_copy;
    va_start(args, fmt);
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy) + 1;
    va_end(args_copy);
    char *ptr = *field ? realloc(*field, len) : malloc(len);
    if(!ptr) {
        va_end(args);
        return 0;
    }
    *field = ptr;
    vsnprintf(*field, len, fmt, args);
    va_end(args);
    return len;
}