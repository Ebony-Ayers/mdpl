#ifndef MDPL_COMMON_HEADER_GUARD
#define MDPL_COMMON_HEADER_GUARD

#include "system_headers.hpp"
#include "file_access.hpp"
#include "buffer.hpp"

//return if the integer return code is non-zero
#define MDPL_RETERR(rc) {int MDPL_RETCODE; if((MDPL_RETCODE = rc) != 0) return MDPL_RETCODE;}

#endif //MDPL_COMMON_HEADER_GUARD