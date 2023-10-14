#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
#define LOG_TEMPLATE_SIZE 100
#define MAX_ARGS_SIZE 1000
#define COLOR_CODES_SIZE_PLUS_NULL 18

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define BLOG_LOG(log, log_type, ...) \
  blog_log(log, log_type, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

typedef enum BLOG_LOG_TYPE { INFO, WARNING, DEBUG, ERROR } BLOG_LOG_TYPE;

char* generate_status_text(BLOG_LOG_TYPE type) {
  char* status_text = "";

  switch (type) {
    case INFO: status_text = "\033[1;32mINFO\033[0m"; break;
    case WARNING: status_text = "\033[1;33mWARNING\033[0m"; break;
    case DEBUG: status_text = "\033[0;34mDEBUG\033[0m"; break;
    case ERROR: status_text = "\033[1;31mERROR\033[0m"; break;
  }

  return status_text;
}

char* generate_status_colored_text(const char* text, BLOG_LOG_TYPE type) {
  char* colored_text = (char*)malloc(strlen(text) + COLOR_CODES_SIZE_PLUS_NULL);

  switch (type) {
    case INFO:
      snprintf(colored_text,
               strlen(text) + COLOR_CODES_SIZE_PLUS_NULL,
               "%s%s%s",
               "\033[1;32m",
               text,
               "\033[0m");
      break;
    case WARNING:
      snprintf(colored_text,
               strlen(text) + COLOR_CODES_SIZE_PLUS_NULL,
               "%s%s%s",
               "\033[1;33m",
               text,
               "\033[0m");
      break;
    case DEBUG:
      snprintf(colored_text,
               strlen(text) + COLOR_CODES_SIZE_PLUS_NULL,
               "%s%s%s",
               "\033[1;34m",
               text,
               "\033[0m");
      break;
    case ERROR:
      snprintf(colored_text,
               strlen(text) + COLOR_CODES_SIZE_PLUS_NULL,
               "%s%s%s",
               "\033[1;31m",
               text,
               "\033[0m");
      break;
  }

  return colored_text;
}

void blog_log(const char* log,
              BLOG_LOG_TYPE log_type,
              const char* calling_file_name,
              const char* calling_function_name,
              int calling_line_number,
              ...) {
  va_list args;
  va_start(args, log);

  char formatted_log[strlen(log) + MAX_ARGS_SIZE];

  vsnprintf(formatted_log, sizeof(formatted_log), log, args);

  time_t t = time(NULL);
  struct tm time = *localtime(&t);

  char* colored_formatted_log = generate_status_colored_text(formatted_log, log_type);

  char log_template[LOG_TEMPLATE_SIZE];
  snprintf(log_template,
           sizeof(log_template),
           "[%s] [%02d/%02d/%d] [%02d:%02d:%02d] %s from %s():%d -> ",
           calling_file_name,
           time.tm_mday,
           time.tm_mon + 1,
           time.tm_year + 1900,
           time.tm_hour,
           time.tm_min,
           time.tm_sec,
           generate_status_text(log_type),
           calling_function_name,
           calling_line_number);

  char final_log[strlen(log_template) + strlen(colored_formatted_log) + 1];

  snprintf(final_log, sizeof(final_log), "%s%s", log_template, colored_formatted_log);

  printf("%s\n", final_log);

  va_end(args);

  free(colored_formatted_log);
}

#define generate_status_text(x) this_is_a_private_function
#define generate_status_colored_text(x) this_is_a_private_function

#endif
