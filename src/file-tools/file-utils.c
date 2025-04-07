#include "file-utils.h"

void append_to_file( struct logger_ctx* ctx, const char* msg ) {
        if ( NULL == ctx->file || NULL == msg ) {
                ctx->status = INVALID_PARAM;
                return;
        }

        ctx->file = fopen( ctx->path, "a" );
        if ( NULL == ctx->file ) {
                ctx->status = FAILED_TO_OPEN;
                return;
        }

        if ( EOF == fputs( msg, ctx->file ) ) {
                fclose( ctx->file );
                ctx->status = WRITE_ERROR;
                return;
        }

        if ( fclose( ctx->file ) != 0 ) {
                ctx->status = FAILED_TO_CLOSE;
                return;
        }

        ctx->status = SUCCESS;
}

int create_directory( const char* path ) {
        if ( NULL == path ) {
                return INVALID_PARAM;
        }

        if ( mkdir( path, 0777 ) == -1 ) {
                if ( errno != EEXIST ) {
                        return FAILED_TO_CREATE_DIR;
                }
        }

        return SUCCESS;
}
