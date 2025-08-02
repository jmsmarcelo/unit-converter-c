#ifndef UNIT_CONVERTER_H
#define UNIT_CONVERTER_H

typedef struct {
    char category[12];
    long double value;
    char unit_from[8];
    char unit_to[8];
} convert_params;
int length_converter(convert_params *params, long double *result);
int weight_converter(convert_params *params, long double *result);
int temperature_converter(convert_params *params, long double *result);

#endif