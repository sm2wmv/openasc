/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#ifndef __LOGGER_H
#define __LOGGER_H
#include <syslog.h>

extern void init_log(char *name);
extern void log_debug(char *format, ...);
extern void log_info(char *format, ...);

#endif
