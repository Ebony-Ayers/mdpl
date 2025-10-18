#ifndef SHORT_LIST_HEADER_GUARD
#define SHORT_LIST_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "../runtime_lib/allocator.hpp"

#include <cstdint>

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
    size_t size;
    size_t capacity;
} MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortListHeader);

//ShortList
typedef struct
{
    MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortListHeader) header;
    MDPL_GENERIC_TYPE_MACRO data[1];
} MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList);

int constructor(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list);
int destructor(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);

int MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, front)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst);
int back(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst);
int get(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const size_t& pos, MDPL_GENERIC_TYPE_MACRO* dst);

int set(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const size_t& pos, const MDPL_GENERIC_TYPE_MACRO& val);

bool isEmpty(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list);
size_t size(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list);
size_t capacity(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list);

int reserve(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const size_t newCapacity);
int clear(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);

int append(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO& val);
int prepend(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO& val);

int removeFront(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);
int removeBack(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);

int allocate(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const size_t& capacity);
int rellocate(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const size_t& capacity, const size_t& offset);
//returns number of bytes
size_t runtimeSizeOf(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list);
//converts number of elements to number of bytes
size_t calculateNewSizeOf(size_t size);
//converts number of bytes to number of bytes
size_t trueSizeToUsableSize(size_t size);

#undef MDPL_GENERIC_TYPE_MACRO
#undef MDPL_GENERIC_TYPE_NAME_MACRO

#endif //SHORT_LIST_HEADER_GUARD