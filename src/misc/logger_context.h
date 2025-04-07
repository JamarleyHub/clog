#ifndef LOGGER_CONTEXT_H
#define LOGGER_CONTEXT_H

#include "error-types.h"

#include <stdio.h>

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

#define GET_STATUS( ctx ) ( ctx->status )

#endif // LOGGER_CONTEXT_H
