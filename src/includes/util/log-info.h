#ifndef LOG_INFO_H
#define LOG_INFO_H

#include <stdio.h>           
#include "includes/util/serial.h"         

typedef enum result_t {
    Ok,
    Error,
    ResultCount
} result_t;

extern const char* result_str[ResultCount];

//background: i initially had these macros on one line, but the line got too long and i just decided to split them for readability
#define SERIAL(status, from, fmt, ...) \
    serial_printf("[  %s  ] %s in %s at line %d: " fmt "", \
                  result_str[status], #from, __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG(status, from, fmt, ...) \
    printf("[  %s  ] %s in %s at line %d: " fmt "", \
           result_str[status], #from, __FILE__, __LINE__, ##__VA_ARGS__)

#endif