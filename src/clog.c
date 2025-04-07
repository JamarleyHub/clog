#include "clog.h"

int logger( struct logger_ctx* ctx, const enum LOG_LEVEL level, const char* fmt, ... ) {
        if ( NULL == ctx || NULL == fmt ) {
                return INVALID_PARAM;
        }
        if ( ctx->default_level < level ) {
                return SUCCESS;
        }

        const char* level_str;
        switch ( level ) {
                case LEVEL_LOG_DEBUG :
                        level_str = LOG_DEBUG_STR;
                        break;
                case LEVEL_LOG_INFO :
                        level_str = LOG_INFO_STR;
                        break;
                case LEVEL_LOG_WARN :
                        level_str = LOG_WARN_STR;
                        break;
                case LEVEL_LOG_ERROR :
                        level_str = LOG_ERROR_STR;
                        break;
                case LEVEL_LOG_FATAL :
                        level_str = LOG_FATAL_STR;
                        break;
                default :
                        level_str = LOG_UNKNOWN_STR;
                        break;
        }

        // Get current time with microsecond precision
        struct timespec ts;
        clock_gettime( CLOCK_REALTIME, &ts );
        const struct tm* tm_info = localtime( &ts.tv_sec );

        char             timestamp[13]; // HH:MM:SS.xx + null terminator
        snprintf( timestamp,
                  sizeof( timestamp ),
                  "%02d:%02d:%02d.%02ld",
                  tm_info->tm_hour,
                  tm_info->tm_min,
                  tm_info->tm_sec,
                  ts.tv_nsec / 10000000 ); // Convert nanoseconds to hundredths

        const int prefix_len =
            (int) ( strlen( level_str ) + strlen( timestamp ) + 5 ); // +5 for " - " and ": "

        // Calculate the length needed for the formatted message
        va_list args;
        va_start( args, fmt );
        const int message_len = vsnprintf( NULL, 0, fmt, args );
        va_end( args );

        // Total length = prefix length + message length + '\0'
        const int total_len = prefix_len + message_len + 1;

        char*     buffer    = malloc( total_len );
        if ( !buffer ) {
                return ALLOC_ERR;
        }

        // Format the prefix with level and timestamp
        snprintf( buffer, total_len, "%s - %s: ", level_str, timestamp );

        // Append the formatted message after the prefix
        va_list args2;
        va_start( args2, fmt );
        vsnprintf( buffer + prefix_len, total_len - prefix_len, fmt, args2 );
        va_end( args2 );

        append_to_file( ctx, buffer );

        free( buffer );

        return ctx->status;
}

struct logger_ctx* register_logger( const enum LOG_LEVEL default_level, const char* path ) {
        struct logger_ctx* ctx = malloc( sizeof( struct logger_ctx ) );
        // Create a  directory and write an empty log into it
        if ( create_directory( path ) != SUCCESS ) {
                ctx->status = FAILED_TO_CREATE_DIR;
                return ctx;
        }

        // Appends the current time and date to the name of the log file in the form of
        // "log-YYYY-MM-DD_HH-MM-SS.log"
        char* formatted_time = malloc( 20 ); // YYYY-MM-DD_HH-MM-SS + null terminator
        if ( !formatted_time ) {
                ctx->status = ALLOC_ERR;
                return ctx;
        }
        char* log_file_name =
            malloc( strlen( path ) + 29 ); // path + /log- + formatted time + .log + null
        if ( !log_file_name ) {
                ctx->status = ALLOC_ERR;
                return ctx;
        }
        const time_t     now = time( NULL );
        const struct tm* t   = localtime( &now );
        strftime( log_file_name, 20, "%Y-%m-%d_%H-%M-%S", t );
        snprintf( log_file_name, strlen( path ) + 29, "%s/log-%s.log", path, formatted_time );
        free( formatted_time );

        // Creates a logging context with the path and a file pointer
        ctx->path          = log_file_name;
        ctx->default_level = default_level;
        ctx->file          = NULL;
        ctx->status        = SUCCESS;

        append_to_file( ctx, INIT_LOG );
        if ( ctx->status != SUCCESS ) {
                free( ctx->path );
                ctx->status = FAILED_TO_CREATE_LOG;
                return ctx;
        }

        return ctx;
}

