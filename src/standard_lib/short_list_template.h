#ifndef SHORT_LIST_HEADER_GUARD
#define SHORT_LIST_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "../runtime_lib/allocator.hpp"

#include <stdint.h>

#define MDPL_SHORT_LIST_INITIAL_CAPACITY 256


//to generate a specialisation of the tempate you have to make a header including the tempalte header and defining the two macros MDPL_GENERIC_TYPE_NAME_MACRO and MDPL_GENERIC_TYPE_MACRO
//the same applies to the cpp file. the hpp and cpp files creates can be used as if they were not generic. MDPL_GENERIC_TYPE_MACRO is the type the list will store. 
//MDPL_GENERIC_TYPE_NAME_MACRO is the namesapce the specialisation will be in. For clarity it is recomended that this name be associated with the type. If the instructions are followed the
//code should compile with out error. If you get an something undefined error make sure that the two macros are defined before including both the hpp and cpp for a total of 4 hash defines.

//unless otherwise stated all instance of ShortList** refer to a pointer to a pointer and not an array. This is done as so that a function can return a pointer to the list.

//positional insert and remove have intentionally not been included. This is to allow reference stability.

//ShortListHeader
typedef struct
{
    uint32_t size;
    uint32_t capacity;
} MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortListHeader);

//ShortList
typedef struct
{
    MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortListHeader) header;
    MDPL_GENERIC_TYPE_MACRO data[1];
} MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList);

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, constructor)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list);
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, destructor)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, front)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst);
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, back)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst);
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, get)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const uint32_t pos, MDPL_GENERIC_TYPE_MACRO* dst);

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, set)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const uint32_t pos, const MDPL_GENERIC_TYPE_MACRO* val);

bool MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, isEmpty)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list);
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, size)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list);
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, capacity)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list);

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, reserve)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const uint32_t newCapacity);
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, clear)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, append)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO* val);
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, prepend)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO* val);

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, removeFront)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, removeBack)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, allocate)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const uint32_t capacity);
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, rellocate)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const uint32_t capacity, const uint32_t offset);
//returns number of bytes
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, runtimeSizeOf)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);
//converts number of elements to number of bytes
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, calculateNewSizeOf)(uint32_t size);
//converts number of bytes to number of bytes
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, trueSizeToUsableSize)(uint32_t size);

#undef MDPL_GENERIC_TYPE_MACRO
#undef MDPL_GENERIC_TYPE_NAME_MACRO

#endif //SHORT_LIST_HEADER_GUARD