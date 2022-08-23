#ifndef _LOG_H_
#define _LOG_H_ 

#include <stdbool.h>

void log_error(const char *function_tag, const char * message, ...);

void log_info(const char *function_tag, const char * message, ...);

void log_warn(const char *function_tag, const char * message, ...);

void log_test_ptr(bool test, const char *function_name, const char *description);

#endif