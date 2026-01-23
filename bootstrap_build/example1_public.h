//generated from: example1.mdpl

#ifndef EXAMPLE1_PUBLIC_HEADER_GUARD
#define EXAMPLE1_PUBLIC_HEADER_GUARD

#include<stdint.h>
#include<stdbool.h>
#include "mdpl_error.h"
#include "mdpl_string.h"

MDPL_ERROR_Error* exampleOne_add(const int32_t a, const int32_t b, const int32_t* c);
MDPL_ERROR_Error* exampleOne_idiv(const int32_t a, const int32_t b, const int32_t* c, const int32_t* rem);
MDPL_ERROR_Error* exampleOne_imul(const int32_t a, const int32_t b, const int32_t* c);
MDPL_ERROR_Error* exampleOne_incremenet(const int32_t* a);
MDPL_ERROR_Error* exampleOne_decremenet(const int32_t* a);
MDPL_ERROR_Error* exampleOne_noop();
MDPL_ERROR_Error* exampleOne_fHalf(const float a, const float* b);
const MDPL_STDLIB_STRING_StringRef str = "Hello world";

#endif //EXAMPLE1_PUBLIC_HEADER_GUARD
