#ifndef CLOG_H
#define CLOG_H

#include "file-tools/file-utils.h"
#include "misc/logger_context.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INIT_LOG                    "Initializing logfile\n"

#define LOG_DEBUG_STR               "[DEBUG] "
#define LOG_INFO_STR                "[INFO] "
#define LOG_WARN_STR                "[WARN] "
#define LOG_ERROR_STR               "[ERROR] "
#define LOG_FATAL_STR               "[FATAL] "
#define LOG_UNKNOWN_STR             "[UNKNOWN] "

#define CLOG_DEBUG( ctx, fmt, ... ) logger( ctx, LOG_DEBUG, fmt, ##__VA_ARGS__ )
#define CLOG_INFO( ctx, fmt, ... )  logger( ctx, LOG_INFO, fmt, ##__VA_ARGS__ )
#define CLOG_WARN( ctx, fmt, ... )  logger( ctx, LOG_WARN, fmt, ##__VA_ARGS__ )
#define CLOG_ERROR( ctx, fmt, ... ) logger( ctx, LOG_ERROR, fmt, ##__VA_ARGS__ )
#define CLOG_FATAL( ctx, fmt, ... ) logger( ctx, LOG_FATAL, fmt, ##__VA_ARGS__ )

/**
 * Logs a message with a specific log level.
 *
 * @param ctx   The logger context containing the file pointer and path
 * @param level The log level (DEBUG, INFO, WARN, ERROR, FATAL)
 * @param fmt   The format string for the message
 * @param ...   The values to format into the message
 * @return enum ERROR_T to indicate status
 */
int                logger( struct logger_ctx* ctx, enum LOG_LEVEL level, const char* fmt, ... );

/**
 * Registers a logger context with a default log level and path.
 *
 * @param default_level The default log level (DEBUG, INFO, WARN, ERROR, FATAL)
 * @param path         The path to the log file
 * @return A pointer to the logger context
 */
struct logger_ctx* register_logger( enum LOG_LEVEL default_level, const char* path );

#define CLOG_DEBUG( ctx, fmt, ... ) logger( ctx, LOG_DEBUG, fmt, ##__VA_ARGS__ )
#define CLOG_INFO( ctx, fmt, ... )  logger( ctx, LOG_INFO, fmt, ##__VA_ARGS__ )
#define CLOG_WARN( ctx, fmt, ... )  logger( ctx, LOG_WARN, fmt, ##__VA_ARGS__ )
#define CLOG_ERROR( ctx, fmt, ... ) logger( ctx, LOG_ERROR, fmt, ##__VA_ARGS__ )
#define CLOG_FATAL( ctx, fmt, ... ) logger( ctx, LOG_FATAL, fmt, ##__VA_ARGS__ )

#endif // CLOG_H
