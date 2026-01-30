#ifndef MDPL_COMMON_HEADER_GUARD
#define MDPL_COMMON_HEADER_GUARD

#include "../pch.h"

#include "system_headers.h"
#include "str_functions.h"
#include "../runtime_lib/mdpl_error.h"

//return if the integer return code is non-zero
#define MDPL_C_RETERR(rc) {int MDPL_RETCODE; if((MDPL_RETCODE = rc) != 0) return MDPL_RETCODE;}
#define MDPL_RETERR(rc) {MDPL_ERROR_Error* MDPL_RETCODE; if((MDPL_RETCODE = (MDPL_ERROR_Error*)rc) != nullptr) return MDPL_RETCODE;}

void MDPL_PRINT_ERROR(MDPL_ERROR_Error* error);

#define MDPL_CONCAT_INDIRECT(ns, type, name) ns##_##type##_##name
#define MDPL_CONCAT(ns, type, name) MDPL_CONCAT_INDIRECT(ns, type, name)

#endif //MDPL_COMMON_HEADER_GUARD
