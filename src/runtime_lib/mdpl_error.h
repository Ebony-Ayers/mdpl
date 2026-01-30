#ifndef ERROR_HEADER_GUARD
#define ERROR_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"

#include <stdint.h>

typedef struct {
    const char* type;
    const char* msg;
    const char* file;
    const char* function;
    uint64_t lineNum;
    int exitCode;
} MDPL_ERROR_Error;

MDPL_ERROR_Error* MDPL_ERROR_createError(const char* type, const char* msg, const char* file, const char* function, size_t lineNum, int exitCode);
void MDPL_ERROR_destroyError(MDPL_ERROR_Error* error);

#define MDPL_ERROR_MSG(MSG) MDPL_ERROR_createError("Generic error", MSG, __FILE__, __PRETTY_FUNCTION__, __LINE__, 1)
#define MDPL_ERROR_TYPE_MSG(TYPE, MSG) MDPL_ERROR_createError(TYPE, MSG, __FILE__, __PRETTY_FUNCTION__, __LINE__, 1)
#define MDPL_ERROR_TYPE_MSG_RET(TYPE, MSG, EXIT) MDPL_ERROR_createError(TYPE, MSG, __FILE__, __PRETTY_FUNCTION__, __LINE__, EXIT)

#define MDPL_ERROR_EXIT_CLI_USAGE          64
#define MDPL_ERROR_EXIT_DATA_FORMAT        65
#define MDPL_ERROR_EXIT_CANNOT_OPEN_FILE   66
#define MDPL_ERROR_EXIT_CANNOT_CREATE_FILE 73
#define MDPL_ERROR_EXIT_IO_ERROR           74
#define MDPL_ERROR_EXIT_PERMISSION_DENIED  77

#define MDPL_ERROR_TYPE_GENERIC "Generic error"
#define MDPL_ERROR_TYPE_NOT_IMPLEMENTED "Not implemented error"
#define MDPL_ERROR_TYPE_EXTERNAL "External error"
#define MDPL_ERROR_TYPE_INVALID_ARGUMENT "Invalid argument error"
#define MDPL_ERROR_TYPE_OUT_OF_RANGE "Out of range error"
#define MDPL_ERROR_TYPE_MAXIMUM_SIZE_EXCEDED "Maximum size exceded error"
#define MDPL_ERROR_TYPE_ALLOCATION "Memory allocation error"

#endif //ERROR_HEADER_GUARD