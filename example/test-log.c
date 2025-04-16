#include "test-log.h"

int main( void ) {
        struct logger_ctx* ctx = register_logger( LEVEL_LOG_DEBUG, ".test_dir" );

        if ( GET_STATUS( ctx ) != SUCCESS ) {
                fprintf( stderr, "Error setting up logger - status %d\n", ctx->status );
                return -1;
        }

        CLOG_INFO( ctx, "Logger initialized successfully" );
        CLOG_INFO( ctx, "Path set to %s", ctx->path );
        CLOG_INFO( ctx, "Directory set to %s", ctx->directory );
        CLOG_INFO( ctx, "Max logs set to %d", ctx->max_logs );

        CLOG_DEBUG( ctx, "This is a debug message" );
        CLOG_INFO( ctx, "This is an info message" );
        CLOG_WARN( ctx, "This is a warning message" );
        CLOG_ERROR( ctx, "This is an error message" );
        CLOG_FATAL( ctx, "This is a fatal message" );
        CLOG_DEBUG( ctx, "This is a message with %d %s", 2, "arguments" );

        set_log_level( ctx, LEVEL_LOG_ERROR );
        enum CLOG_LOG_LEVEL lvl = get_log_level( ctx );

        if ( lvl != LEVEL_LOG_ERROR ) {
                fprintf( stderr, "Error setting log level - status %d\n", ctx->status );
                return -1;
        }

        CLOG_DEBUG( ctx, "This is a hidden debug message" );
        CLOG_INFO( ctx, "This is a hidden info message" );
        CLOG_WARN( ctx, "This is a hidden warning message" );
        CLOG_ERROR( ctx, "This is a visible error message" );
        CLOG_FATAL( ctx, "This is a visible fatal message" );
        CLOG_DEBUG( ctx, "This is a hidden message with %d %s", 2, "arguments" );

        unregister_logger( &ctx );

        if ( NULL == ctx ) {
                fprintf( stdout, "Logger unregistered successfully\n" );
        }

        return 0;
}
