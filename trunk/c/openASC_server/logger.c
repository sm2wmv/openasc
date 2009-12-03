/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#include "logger.h"
#include "config.h"
#include <stdarg.h>
#include <syslog.h>

void init_log(char *name)
{
	openlog(name, LOG_PERROR | LOG_PID, LOG_DAEMON);
}

void log_debug(char *format, ...)
{
	va_list args;
	if (conf->debug) {
		va_start(args, format);
		vsyslog(LOG_DEBUG, format, args);
		va_end(args);
	}
}

void log_info(char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsyslog(LOG_INFO, format, args);
	va_end(args);
}
