/*
 * ulog.c
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <core/ulog.h>

static const int max_input_size=1024;

void ulog_init()
{
#ifdef ZLOG
  int ret=-1;
  if ((ret=zlog_init("/etc/log.conf")))
  {
    fprintf(stderr, "%s(): Error in zlog_init(), ret=%d\n", __FUNCTION__, ret);
  }
#endif
}

void ulog_clean()
{
#ifdef ZLOG
  zlog_fini();
#endif
}

void ulog_info(const char* category, const char* file_name, const char* func_name, int line, const char* format, ...)
{
  char buf[max_input_size];

  BUILD_BUFFER(buf, format);

#ifdef ZLOG
  #if ZLOG_VER >= 1
    zlog(zlog_get_category(category), file_name, strlen(file_name), func_name, strlen(func_name), line, ZLOG_LEVEL_INFO, buf);
  #else
    zlog(zlog_get_category(category), file_name, line, ZLOG_LEVEL_INFO, buf);
  #endif
#else
  #ifdef ULOG_INFO
    printf("%s%s%s", INFO, buf, ENDLINE);
  #endif
#endif
}

void ulog_error(const char* category, const char* file_name, const char* func_name, int line, const char* format, ...)
{
  char buf[max_input_size];

  BUILD_BUFFER(buf, format);

#ifdef ZLOG
  #if ZLOG_VER >= 1
    zlog(zlog_get_category(category), file_name, strlen(file_name), func_name, strlen(func_name), line, ZLOG_LEVEL_ERROR, buf);
  #else
    zlog(zlog_get_category(category), file_name, line, ZLOG_LEVEL_ERROR, buf);
  #endif
#else
  fprintf(stderr, "%s%s%s", ERROR, buf, ENDLINE);
#endif
}

void ulog_common(enum zlog_level level, const char* category, const char* file_name, const char* func_name, int line, const char* format, ...)
{
  char buf[max_input_size];

  BUILD_BUFFER(buf, format);
#ifdef ZLOG
  #if ZLOG_VER >= 1
    zlog(zlog_get_category(category), file_name, strlen(file_name), func_name, strlen(func_name), line, level, buf);
  #else
    zlog(zlog_get_category(category), file_name, line, level, buf);
  #endif
#else
  fprintf(stderr, "%s%s%s", WARN, buf, ENDLINE);
#endif
}


