#ifndef CLOG_H
#define CLOG_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define GET_STATUS( ctx )           ( ctx->status )

#define INIT_LOG                    "Initializing logfile\n"

#define LOG_DEBUG_STR               "[DEBUG]"
#define LOG_INFO_STR                "[INFO]"
#define LOG_WARN_STR                "[WARN]"
#define LOG_ERROR_STR               "[ERROR]"
#define LOG_FATAL_STR               "[FATAL]"
#define LOG_UNKNOWN_STR             "[UNKNOWN]"

#define CLOG_DEBUG( ctx, fmt, ... ) logger( ctx, LEVEL_LOG_DEBUG, fmt, ##__VA_ARGS__ )
#define CLOG_INFO( ctx, fmt, ... )  logger( ctx, LEVEL_LOG_INFO, fmt, ##__VA_ARGS__ )
#define CLOG_WARN( ctx, fmt, ... )  logger( ctx, LEVEL_LOG_WARN, fmt, ##__VA_ARGS__ )
#define CLOG_ERROR( ctx, fmt, ... ) logger( ctx, LEVEL_LOG_ERROR, fmt, ##__VA_ARGS__ )
#define CLOG_FATAL( ctx, fmt, ... ) logger( ctx, LEVEL_LOG_FATAL, fmt, ##__VA_ARGS__ )

enum ERROR_T
{
        SUCCESS              = 0,
        GENERIC_ERR          = -1,
        INVALID_PARAM        = -2,
        FAILED_TO_OPEN       = -3,
        WRITE_ERROR          = -4,
        FAILED_TO_CLOSE      = -5,
        FAILED_TO_CREATE_DIR = -6,
        FAILED_TO_CREATE_LOG = -7,
        ALLOC_ERR            = -8,
};

enum LOG_LEVEL
{
        LEVEL_LOG_DEBUG = 0,
        LEVEL_LOG_INFO,
        LEVEL_LOG_WARN,
        LEVEL_LOG_ERROR,
        LEVEL_LOG_FATAL
};

struct logger_ctx
{
        char*          path;
        enum LOG_LEVEL default_level;
        FILE*          file;
        enum ERROR_T   status;
};

/**
 * Appends a message to a log file.
 *
 * @param ctx The logger context containing the file pointer and path
 * @param msg The message to write to it
 * @return enum ERROR_T to indicate status
 */
void               append_to_file( struct logger_ctx* ctx, const char* msg );

/**
 * Creates a directory.
 *
 * @param path The path to the directory
 * @return enum ERROR_T to indicate status
 */
int                create_directory( const char* path );

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

#endif // CLOG_H
