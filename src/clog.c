#include "clog.h"

/**
 * Appends a message to a log file.
 *
 * @param ctx The logger context containing the file pointer and path - The return status is saved
 * in the status field of the context
 * @param msg The message to write to it
 */
__LIB_INTERNAL void append_to_file( struct logger_ctx* ctx, const char* msg ) {
        if ( pthread_mutex_lock( &ctx->mutex ) != 0 ) {
                return;
        }

        if ( NULL == msg ) {
                ctx->status = INVALID_PARAM;
                pthread_mutex_unlock( &ctx->mutex );
                return;
        }

        ctx->file = fopen( ctx->path, "a" );
        if ( NULL == ctx->file ) {
                ctx->status = FAILED_TO_OPEN;
                pthread_mutex_unlock( &ctx->mutex );
                return;
        }

        if ( EOF == fputs( msg, ctx->file ) ) {
                fclose( ctx->file );
                ctx->status = WRITE_ERROR;
                pthread_mutex_unlock( &ctx->mutex );
                return;
        }

        if ( fclose( ctx->file ) != 0 ) {
                ctx->status = FAILED_TO_CLOSE;
                pthread_mutex_unlock( &ctx->mutex );
                return;
        }

        ctx->status = SUCCESS;
        pthread_mutex_unlock( &ctx->mutex );
}

/**
 * Creates a directory.
 *
 * @param path The path to the directory
 * @return enum CLOG_ERROR_T to indicate status
 */
__LIB_INTERNAL enum CLOG_ERROR_T create_directory( const char* path ) {
        if ( NULL == path ) {
                return INVALID_PARAM;
        }
        if ( -1 == mkdir( path, 0777 ) ) {
                if ( errno != EEXIST ) {
                        return FAILED_TO_CREATE_DIR;
                }
        }

        return SUCCESS;
}

/**
 * Returns a current timestamp in the format HH:MM:SS.xx
 *
 * @return A malloc'd string with the timestamp
 */
__LIB_INTERNAL char* get_timestamp( void ) {
        // Get current time with microsecond precision
        struct timespec ts;
        clock_gettime( CLOCK_REALTIME, &ts );
        const struct tm* tm_info       = localtime( &ts.tv_sec );

        const int        len_timestamp = 12; // HH:MM:SS.xx + null terminator
        char*            timestamp     = malloc( len_timestamp * sizeof( char ) );
        if ( NULL == timestamp ) {
                return NULL;
        }
        snprintf( timestamp,
                  len_timestamp,
                  "%02d:%02d:%02d.%02ld",
                  tm_info->tm_hour,
                  tm_info->tm_min,
                  tm_info->tm_sec,
                  ts.tv_nsec / 10000000 ); // Convert nanoseconds to hundredths
        return timestamp;
}

/**
 * Compare function for quicksort
 *
 * @param a String a
 * @param b String b
 * @return String-comparison result
 */
__LIB_INTERNAL int cmp( const void* a, const void* b ) {
        const char* s1 = *(const char**) a;
        const char* s2 = *(const char**) b;
        return strcmp( s1, s2 );
}

/**
 * Cleans up old logs in the log directory
 *
 * @param ctx The logger context
 * @return enum CLOG_ERROR_T to indicate status
 */
__LIB_INTERNAL enum CLOG_ERROR_T cleanup_old_logs( struct logger_ctx* ctx ) {
        if ( NULL == ctx ) {
                return INVALID_PARAM;
        }
        DIR* dp = opendir( ctx->directory );
        if ( NULL == dp ) {
                ctx->status = FAILED_TO_OPEN;
                return ctx->status;
        }
        size_t         count = 0;
        struct dirent* ep;
        char**         logs = NULL;

        // Count how many logs are in the log directory
        while ( ( ep = readdir( dp ) ) ) {
                if ( strncmp( ep->d_name, "log-", 4 ) != 0 ) {
                        continue;
                }
                size_t len = strlen( ep->d_name );
                if ( len != 27 || strcmp( ep->d_name + len - 4, ".log" ) != 0 ) {
                        continue;
                }
                char** tmp = realloc( logs, ( count + 1 ) * sizeof( char* ) );
                if ( NULL == tmp ) {
                        for ( size_t i = 0; i < count; i++ ) {
                                free( logs[i] );
                        }
                        free( logs );
                        closedir( dp );
                        ctx->status = ALLOC_ERR;
                        return ctx->status;
                }

                logs        = tmp;
                logs[count] = strdup( ep->d_name );
                if ( NULL == logs[count] ) {
                        for ( size_t i = 0; i < count; i++ ) {
                                free( logs[i] );
                        }
                        free( logs );
                        closedir( dp );
                        ctx->status = ALLOC_ERR;
                        return ctx->status;
                }
                count++;
        }
        closedir( dp );

        // If we haven't reached the max number of logs, we can return
        if ( count <= ctx->max_logs ) {
                for ( size_t i = 0; i < count; i++ ) {
                        free( logs[i] );
                }
                free( logs );
                ctx->status = SUCCESS;
                return ctx->status;
        }

        // Sort lexicographically and remove the oldest logs
        qsort( logs, count, sizeof( char* ), cmp );
        size_t num_delete = count - ctx->max_logs;
        for ( size_t i = 0; i < num_delete; i++ ) {
                char full_path[PATH_MAX];
                snprintf( full_path, sizeof( full_path ), "%s/%s", ctx->directory, logs[i] );
                if ( remove( full_path ) != 0 ) {
                        ctx->status = WRITE_ERROR;
                }
        }

        // Cleanup
        for ( size_t i = 0; i < count; i++ ) {
                free( logs[i] );
        }
        free( logs );
        return ctx->status;
}

// ------------------------------------------------------------------------ //
// ------------------------------ PUBLIC API ------------------------------ //
// ------------------------------------------------------------------------ //

enum CLOG_ERROR_T
logger( struct logger_ctx* ctx, const enum CLOG_LOG_LEVEL level, const char* fmt, ... )
{
        if ( NULL == ctx || NULL == fmt ) {
                return INVALID_PARAM;
        }
        if ( ctx->default_level > level ) {
                ctx->status = SUCCESS;
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

        char* timestamp = get_timestamp( );
        if ( NULL == timestamp ) {
                timestamp = "00:00:00.00";
        }

        const int prefix_len = (int) ( strlen( timestamp ) + 3 + strlen( level_str )
                                       + 2 ); // +3 for " - " and +3 for ": " with

        // Calculate the length needed for the formatted message
        va_list   args;
        va_start( args, fmt );
        const int message_len = vsnprintf( NULL, 0, fmt, args );
        va_end( args );

        // Total length = prefix length + message length + '\n\0'
        const int total_len = prefix_len + message_len + 2;

        char*     buffer    = malloc( total_len );
        if ( !buffer ) {
                ctx->status = ALLOC_ERR;
                return ALLOC_ERR;
        }

        // Format the prefix with timestamp and level
        snprintf( buffer, total_len, "%s - %s: ", timestamp, level_str );
        free( timestamp );

        // Append the formatted message after the prefix
        va_list args2;
        va_start( args2, fmt );
        vsnprintf( buffer + prefix_len, total_len - prefix_len, fmt, args2 );
        va_end( args2 );

        strcat( buffer, "\n" );

        append_to_file( ctx, buffer );

        free( buffer );

        return ctx->status;
}

enum CLOG_LOG_LEVEL get_log_level( struct logger_ctx* ctx )
{
        if ( NULL == ctx ) {
                return LEVEL_LOG_DEBUG;
        }
        return ctx->default_level;
}

enum CLOG_ERROR_T set_log_level( struct logger_ctx* ctx, const enum CLOG_LOG_LEVEL level )
{
        if ( NULL == ctx ) {
                return INVALID_PARAM;
        }

        ctx->default_level = level;
        ctx->status        = SUCCESS;
        return ctx->status;
}

struct logger_ctx* register_logger( const enum CLOG_LOG_LEVEL default_level,
                                    const char*               directory ) {
        struct logger_ctx* ctx = malloc( sizeof( struct logger_ctx ) );
        if ( NULL == ctx ) {
                return NULL;
        }

        ctx->status = UNINITIALIZED;

        if ( NULL == directory ) {
                ctx->status = INVALID_PARAM;
                return ctx;
        }

        // Create a directory and write an empty log into it
        if ( create_directory( directory ) != SUCCESS ) {
                ctx->status = FAILED_TO_CREATE_DIR;
                return ctx;
        }

        // Appends the current time and date to the name of the log file in the form of
        // "log-YYYY-MM-DD_HH-MM-SS.log"
        char* formatted_time = malloc( 20 ); // YYYY-MM-DD_HH-MM-SS + null terminator
        if ( NULL == formatted_time ) {
                ctx->status = ALLOC_ERR;
                return ctx;
        }
        char* log_file_name =
            malloc( strlen( directory ) + 29 ); // path + /log- + formatted time + .log + null
        if ( NULL == log_file_name ) {
                ctx->status = ALLOC_ERR;
                return ctx;
        }
        const time_t     now = time( NULL );
        const struct tm* t   = localtime( &now );
        strftime( formatted_time, 20, "%Y-%m-%d_%H-%M-%S", t );
        snprintf( log_file_name,
                  strlen( directory ) + 29,
                  "%s/log-%s.log",
                  directory,
                  formatted_time );
        free( formatted_time );

        // Creates a logging context with the path and a file pointer
        ctx->path      = log_file_name;
        ctx->directory = strdup( directory );
        if ( NULL == ctx->directory ) {
                ctx->status = ALLOC_ERR;
                free( ctx->path );
                return ctx;
        }
        ctx->default_level = default_level;
        ctx->file          = NULL;
        ctx->max_logs      = CLOG_MAX_LOG_FILES;
        ctx->status        = SUCCESS;
        pthread_mutex_init( &ctx->mutex, NULL );

        char*     timestamp    = get_timestamp( );
        const int init_str_len = (int) ( strlen( INIT_LOG ) + strlen( timestamp )
                                         + 5 ); // + 3 for " - " and + 2 for '\n\0'
        char*     init_log_str = malloc( init_str_len * sizeof( char ) );
        if ( NULL == init_log_str ) {
                ctx->status = ALLOC_ERR;
                free( ctx->path );
                return ctx;
        }
        snprintf( init_log_str, init_str_len, "%s - %s", timestamp, INIT_LOG );

        append_to_file( ctx, init_log_str );

        cleanup_old_logs( ctx );

        if ( ctx->status != SUCCESS ) {
                free( ctx->path );
                return ctx;
        }

        return ctx;
}

void unregister_logger( struct logger_ctx** ctx ) {
        pthread_mutex_lock( &( *ctx )->mutex );
        free( ( *ctx )->path );
        free( ( *ctx )->directory );
        ( *ctx )->status = UNINITIALIZED;
        pthread_mutex_unlock( &( *ctx )->mutex );
        free( *ctx );
        *ctx = NULL;
}
