#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void log_error(const char *function_tag, const char * message)
{
    time_t now;
    time(&now);
    fprintf(stderr, "[%s][ERROR][%s]:%s\n", ctime(&now), function_tag, message);
}

void log_info(const char *function_tag, const char * message)
{
    time_t now;
    time(&now);
    fprintf(stderr, "[%s][INFO][%s]:%s\n", ctime(&now), function_tag, message);
}

void log_warn(const char *function_tag, const char * message)
{
    time_t now;
    time(&now);
    fprintf(stderr, "[%s][WARN][%s]:%s\n", ctime(&now), function_tag, message);
}
