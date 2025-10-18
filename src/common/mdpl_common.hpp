#ifndef MDPL_COMMON_HEADER_GUARD
#define MDPL_COMMON_HEADER_GUARD

#include "../pch.hpp"

#include "system_headers.hpp"
#include "file_access.hpp"
#include "buffer.hpp"
#include "str_functions.hpp"

//return if the integer return code is non-zero
#define MDPL_RETERR(rc) {int MDPL_RETCODE; if((MDPL_RETCODE = rc) != 0) return MDPL_RETCODE;}

#define MDPL_CONCAT_INDIRECT(ns, type, name) ns##_##type##_##name
#define MDPL_CONCAT(ns, type, name) MDPL_CONCAT_INDIRECT(ns, type, name)

#endif //MDPL_COMMON_HEADER_GUARD
