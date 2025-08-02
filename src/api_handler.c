#include <string.h>
#include <stdlib.h>
#include "../include/api_handler.h"
#include "../include/unit_converter.h"
#include "../include/server_utils.h"
#include "../include/http_response.h"

int parse_json(const char *json, convert_params *params) {
    char *endptr;
    char value[128];
    if(!get_json_value(json, "category", params->category, sizeof(params->category))) return 0;
    if(get_json_value(json, "value", value, sizeof(value))) {
        params->value = strtold(value, &endptr);
        if(value == endptr || *endptr != '\0') return 0;
    } else {
        return 0;
    }
    if(!get_json_value(json, "unit_from", params->unit_from, sizeof(params->unit_from))) return 0;
    if(!get_json_value(json, "unit_to", params->unit_to, sizeof(params->unit_to))) return 0;
    return 1;
}
void handle_convert(client_header *header) {
    convert_params params;
    int status;
    if(parse_json(header->body_request, &params)) {
        long double result = 0.0;
        if(strcmp(params.category, "Length") == 0) {
            status = length_converter(&params, &result);
        } else if(strcmp(params.category, "Weight") == 0) {
            status = weight_converter(&params, &result);
        } else if(strcmp(params.category, "Temperature") == 0) {
            status = temperature_converter(&params, &result);
        } else {
            status = 1;
        }
        switch(status) {
            case 0:
                set_formatted_string(&(header->body_response),
                    (result < 1e-6 || result > 1e+6) ? "{\"value\":%Le}" : "{\"value\":%.15Lf}", result);
                return;
            case 1:
                set_formatted_string(&(header->body_response), "{\"message\":\"Invalid category: %s\"}", params.category);
                break;
            case 2:
                set_formatted_string(&(header->body_response), "{\"message\":\"Invalid source unit (unit_from): %s\"}", params.unit_from);
                break;
            case 3:
                set_formatted_string(&(header->body_response), "{\"message\":\"Invalid target unit (unit_to): %s\"}", params.unit_to);
                break;
            case 4:
                set_formatted_string(&(header->body_response), "{\"message\":\"Invalid value: %.0Lf\"}", params.value);
                break;
            default:
                set_string(&(header->body_response), "{\"message\":\"Invalid conversion request\"}");
        }
    } else {
        set_string(&(header->body_response), "{\"message\":\"Malformed JSON in resquest body\"}");
    }
    header->status_code = 400;
}
void handle_api(client_header *header) {
    if(strcmp(header->method, "POST") != 0) {
        header->status_code = 405;
        set_formatted_string(&(header->status_msg), "The %s method is not allowed", header->method);
    }
    strcpy(header->content_type, "application/json");
    if(header->status_code >= 400) {
        if(!header->status_msg) set_default_status_msg(header);
        set_formatted_string(&(header->body_response), "{\"message\":\"%s\"}", header->status_msg);
    } else if(strcmp(header->path, "/api/unit-converter") == 0) {
        if(!header->body_request) {
            header->status_code = 400;
            set_string(&(header->body_response), "{\"message\":\"No request body\"}");
        } else {
            unit_converter(header);
        }
    } else {
        header->status_code = 404;
        set_formatted_string(&(header->body_response), "{\"message\":\"The path '%s' is not valid endpoint\"}", header->path);
    }
    send_response(header, header->body_response);
}