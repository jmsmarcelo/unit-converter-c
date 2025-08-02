#include <string.h>
#include "../include/unit_converter.h"

long double length_to_meters(const char *unit, long double value) {
    if(strcmp(unit, "mm") == 0) return value / 1000.0L;
    if(strcmp(unit, "cm") == 0) return value / 100.0L;
    if(strcmp(unit, "m") == 0) return value;
    if(strcmp(unit, "km") == 0) return value * 1000.0L;
    if(strcmp(unit, "in") == 0) return value * 0.0254L;
    if(strcmp(unit, "ft") == 0) return value * 0.3048L;
    if(strcmp(unit, "yd") == 0) return value * 0.9144L;
    if(strcmp(unit, "mi") == 0) return value * 1609.344L;
    return 0.0;
}
long double meters_to_length(const char *unit, long double value) {
    if(strcmp(unit, "mm") == 0) return value * 1000.0L;
    if(strcmp(unit, "cm") == 0) return value * 100.0L;
    if(strcmp(unit, "m") == 0) return value;
    if(strcmp(unit, "km") == 0) return value / 1000.0L;
    if(strcmp(unit, "in") == 0) return value / 0.0254L;
    if(strcmp(unit, "ft") == 0) return value / 0.3048L;
    if(strcmp(unit, "yd") == 0) return value / 0.9144L;
    if(strcmp(unit, "mi") == 0) return value / 1609.344L;
    return 0.0;
}
int length_converter(convert_params *params, long double *result) {
    if(params->value == 0.0L) return 4;
    *result = length_to_meters(params->unit_from, params->value);
    if(*result == 0.0L) return 2;
    *result = meters_to_length(params->unit_to, *result);
    if(*result == 0.0L) return 3;
    return 0;
}
long double weight_to_grams(const char *unit, long double value) {
    if(strcmp(unit, "µg") == 0) return value / 1000000.0L;
    if(strcmp(unit, "mg") == 0) return value / 1000.0L;
    if(strcmp(unit, "g") == 0) return value;
    if(strcmp(unit, "kg") == 0) return value * 1000.0L;
    if(strcmp(unit, "t") == 0) return value * 1000000.0L;
    if(strcmp(unit, "oz") == 0) return value * 28.3495L;
    if(strcmp(unit, "lb") == 0) return value * 453.592L;
    return 0.0;
}
long double grams_to_weight(const char *unit, long double value) {
    if(strcmp(unit, "µg") == 0) return value * 1000000.0L;
    if(strcmp(unit, "mg") == 0) return value * 1000.0L;
    if(strcmp(unit, "g") == 0) return value;
    if(strcmp(unit, "kg") == 0) return value / 1000.0L;
    if(strcmp(unit, "t") == 0) return value / 1000000.0L;
    if(strcmp(unit, "oz") == 0) return value / 28.3495L;
    if(strcmp(unit, "lb") == 0) return value / 453.592L;
    return 0.0;
}
int weight_converter(convert_params *params, long double *result) {
    if(params->value == 0.0L) return 4;
    *result = weight_to_grams(params->unit_from, params->value);
    if(*result == 0.0L) return 2;
    *result = grams_to_weight(params->unit_to, *result);
    if(*result == 0.0L) return 3;
    return 0;
}
int temperature_converter(convert_params *params, long double *result) {
    long double value = params->value;
    if(strcmp(params->unit_from, "°C") == 0) {
        if(strcmp(params->unit_to, "°C") == 0) *result = value;
        else if(strcmp(params->unit_to, "°F") == 0) *result = (value * 9.0L / 5.0L) + 32.0L;
        else if(strcmp(params->unit_to, "K") == 0) *result = value + 273.15L;
        else return 3;
    } else if(strcmp(params->unit_from, "°F") == 0) {
        if(strcmp(params->unit_to, "°C") == 0) *result = (value - 32.0L) * 5.0L / 9.0L;
        else if(strcmp(params->unit_to, "°F") == 0) *result = value;
        else if(strcmp(params->unit_to, "K") == 0) *result = ((value - 32.0L) * 5.0L / 9.0L) + 273.15L;
        else return 3;
    } else if(strcmp(params->unit_from, "K") == 0) {
        if(strcmp(params->unit_to, "°C") == 0) *result = value - 273.15L;
        else if(strcmp(params->unit_to, "°F") == 0) *result = ((value - 273.15L) * 9.0L / 5.0L) + 32.0L;
        else if(strcmp(params->unit_to, "K") == 0) *result = value;
        else return 3;
    } else {
        return 2;
    }
    return 0;
}