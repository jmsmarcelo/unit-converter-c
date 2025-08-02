#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

const char *extract_until(const char *src, const char *delim, char **out, size_t *out_cap);
int set_string(char **field, const char *value);
void set_formatted_string(char **field, const char *fmt, ...);

#endif