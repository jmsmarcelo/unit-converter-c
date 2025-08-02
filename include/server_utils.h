#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

const char *extract_until(const char *src, const char *delim, char **out, size_t *out_cap);
int set_string(char **field, const char *value);
int set_formatted_string(char **field, const char *fmt, ...);
int get_json_value(const char *json, char *key, char *value, size_t value_len);

#endif