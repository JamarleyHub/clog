#ifndef CLOG_H
#define CLOG_H

#include "logging/logger.h"
#include "misc/error-types.h"
#include "misc/logger_context.h"

#define LOG_DEBUG( ctx, fmt, ... ) logger( ctx, LOG_DEBUG, fmt, ##__VA_ARGS__ )
#define LOG_INFO( ctx, fmt, ... )  logger( ctx, LOG_INFO, fmt, ##__VA_ARGS__ )
#define LOG_WARN( ctx, fmt, ... )  logger( ctx, LOG_WARN, fmt, ##__VA_ARGS__ )
#define LOG_ERROR( ctx, fmt, ... ) logger( ctx, LOG_ERROR, fmt, ##__VA_ARGS__ )
#define LOG_FATAL( ctx, fmt, ... ) logger( ctx, LOG_FATAL, fmt, ##__VA_ARGS__ )

#endif // CLOG_H
