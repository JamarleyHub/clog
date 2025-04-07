#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

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

#endif // ERROR_TYPES_H
