#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../misc/logger_context.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * Appends a message to a log file.
 *
 * @param ctx The logger context containing the file pointer and path
 * @param msg The message to write to it
 * @return enum ERROR_T to indicate status
 */
void append_to_file( struct logger_ctx* ctx, const char* msg );

/**
 * Creates a directory.
 *
 * @param path The path to the directory
 * @return enum ERROR_T to indicate status
 */
int  create_directory( const char* path );

#endif // FILE_UTILS_H
