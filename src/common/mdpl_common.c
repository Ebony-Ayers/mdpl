#include "mdpl_common.h"

void MDPL_PRINT_ERROR(MDPL_ERROR_Error* error)
{
    if(error != nullptr)
    {
        printf("%s: %s.\nLine: %lu, function: %s, file: %s.\n", error->type, error->msg, error->lineNum, error->function, error->file);
    }
}