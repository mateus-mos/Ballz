#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_FMT_SIZE 256 

void log_error(const char *function_tag, const char * message, ...)
{

    char formatted_string[MAX_FMT_SIZE];
    time_t now;
    time(&now);

    struct tm *local = localtime(&now);

    va_list argptr;
    va_start(argptr, message);
    
    vsprintf(formatted_string, message, argptr);

    fprintf(stderr, "[%d:%d:%d][ERROR][%s]:%s\n", local->tm_hour, local->tm_min, local->tm_sec, function_tag, formatted_string);

    va_end(argptr);
}

void log_info(const char *function_tag, const char * message, ...)
{
    char formatted_string[MAX_FMT_SIZE];
    time_t now;
    time(&now);

    struct tm *local = localtime(&now);

    va_list argptr;
    va_start(argptr, message);
    
    vsprintf(formatted_string, message, argptr);

    fprintf(stderr, "[%d:%d:%d][INFO][%s]:%s\n", local->tm_hour, local->tm_min, local->tm_sec, function_tag, formatted_string);

    va_end(argptr);
}

void log_warn(const char *function_tag, const char * message, ...)
{
    char formatted_string[MAX_FMT_SIZE];
    time_t now;
    time(&now);

    struct tm *local = localtime(&now);

    va_list argptr;
    va_start(argptr, message);
    
    vsprintf(formatted_string, message, argptr);

    fprintf(stderr, "[%d:%d:%d][WARN][%s]:%s\n", local->tm_hour, local->tm_min, local->tm_sec, function_tag, formatted_string);
}

void log_test_ptr(bool test, const char *function_tag, const char *ptr_name)
{
    if(test)
        return;

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    fprintf(stderr, "[%d:%d:%d][ERROR][%s]:%s is NULL!\n", local->tm_hour, local->tm_min, local->tm_sec, function_tag, ptr_name);

    exit(1);
}