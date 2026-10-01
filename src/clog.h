#ifndef CLOG_H
#define CLOG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define GET_STATUS( ctx )           ( ctx->status )

#define CLOG_DEFAULT_MAX_LOG_FILES  10

#define __LIB_INTERNAL              __attribute__( ( visibility( "hidden" ) ) )

#define CLOG_DEBUG( ctx, fmt, ... ) log_message( ctx, LEVEL_LOG_DEBUG, fmt, ##__VA_ARGS__ )
#define CLOG_INFO( ctx, fmt, ... )  log_message( ctx, LEVEL_LOG_INFO, fmt, ##__VA_ARGS__ )
#define CLOG_WARN( ctx, fmt, ... )  log_message( ctx, LEVEL_LOG_WARN, fmt, ##__VA_ARGS__ )
#define CLOG_ERROR( ctx, fmt, ... ) log_message( ctx, LEVEL_LOG_ERROR, fmt, ##__VA_ARGS__ )
#define CLOG_FATAL( ctx, fmt, ... ) log_message( ctx, LEVEL_LOG_FATAL, fmt, ##__VA_ARGS__ )

enum CLOG_ERROR_T
{
        UNINITIALIZED        = 1,
        SUCCESS              = 0,
        GENERIC_ERR          = -1,
        INVALID_PARAM        = -2,
        FAILED_TO_OPEN       = -3,
        WRITE_ERROR          = -4,
        FAILED_TO_CLOSE      = -5,
        FAILED_TO_CREATE_DIR = -6,
        FAILED_TO_CREATE_LOG = -7,
        ALLOC_ERR            = -8,
        FAILED_TO_LOCK       = -9,
};

typedef enum
{
        LEVEL_LOG_DEBUG = 0,
        LEVEL_LOG_INFO,
        LEVEL_LOG_WARN,
        LEVEL_LOG_ERROR,
        LEVEL_LOG_FATAL,
        LEVEL_LOG_UNKNOWN,
} CLOG_LOG_LEVEL;

struct text_config
{
        char* log_init_str;
        char* log_debug_str;
        char* log_info_str;
        char* log_warn_str;
        char* log_error_str;
        char* log_fatal_str;
        char* log_unknown_str;
};

struct logger_ctx
{
        pthread_mutex_t    mutex;
        char*              path;
        char*              directory;
        CLOG_LOG_LEVEL     default_level;
        FILE*              file;
        uint8_t            max_logs;
        enum CLOG_ERROR_T  status;
        struct text_config text_config;
};

/**
 * Logs a message with a specific log level.
 *
 * @param ctx   The logger context
 * @param level The log level (DEBUG, INFO, WARN, ERROR, FATAL)
 * @param fmt   The format string for the message
 * @param ...   The values to format into the message
 * @return enum CLOG_ERROR_T to indicate status
 */
enum CLOG_ERROR_T log_message( struct logger_ctx* ctx, CLOG_LOG_LEVEL level, const char* fmt, ... );

/**
 * Gets the log level for a logger context.
 *
 * @param ctx The logger context
 * @return enum CLOG_LOG_LEVEL to indicate current log level
 */
CLOG_LOG_LEVEL    get_log_level( const struct logger_ctx* ctx );

/**
 * Sets the log level for a logger context.
 *
 * @param ctx   The logger context
 * @param level The log level to set to (DEBUG, INFO, WARN, ERROR, FATAL)
 * @return enum CLOG_ERROR_T to indicate status
 */
enum CLOG_ERROR_T set_log_level( struct logger_ctx* ctx, CLOG_LOG_LEVEL level );

/**
 * Registers a logger context with a default log level and path.
 *
 * @param default_level The default log level (DEBUG, INFO, WARN, ERROR, FATAL)
 * @param path         The path to the log file
 * @return A pointer to the logger context
 */
struct logger_ctx* register_logger( CLOG_LOG_LEVEL default_level, const char* path );

/**
 * Change the text that is being displayed in the log file before the individual
 * log levels.
 *
 * @param ctx The logger context
 * @param level The level for which to change the logging text
 * @param new_str The new logging text
 */
void change_logging_strings( struct logger_ctx* ctx, CLOG_LOG_LEVEL level, char* new_str );

/**
 * Change the retention policy for how many old log files you want to keep
 *
 * @param ctx The logger context
 * @param num The number of logfiles to keep (0 disabling the log cleanup)
 */
void change_retained_logfiles( struct logger_ctx* ctx, size_t num );

/**
 * Unregisters a logger context and frees the associated resources.
 *
 * @param ctx The logger context to unregister
 */
void unregister_logger( struct logger_ctx** ctx );

#endif // CLOG_H
