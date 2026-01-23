#include "error.hpp"

MDPL_ERROR_Error* MDPL_ERROR_createError(const char* type, const char* msg, const char* file, const char* function, size_t lineNum, int exitCode)
{
    MDPL_ERROR_Error* error = (MDPL_ERROR_Error*)malloc(sizeof(MDPL_ERROR_Error));
    error->type = type;
    error->msg = msg;
    error->file = file;
    error->function = function;
    error->lineNum = lineNum;
    error->exitCode = exitCode;
    return error;
}
void MDPL_ERROR_destroyError(MDPL_ERROR_Error* error)
{
    if(error != nullptr)
    {
        free(error);
    }
}