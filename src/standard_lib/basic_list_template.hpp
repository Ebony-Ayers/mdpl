#ifndef BASIC_LIST_HEADER_GUARD
#define BASIC_LIST_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "../runtime_lib/allocator.hpp"

#include <cstdint>

#define MDPL_BASIC_LIST_INITIAL_CAPACITY 256

//to generate a specialisation of the tempate you have to make a header including the tempalte header and defining the two macros MDPL_GENERIC_TYPE_NAME_MACRO and MDPL_GENERIC_TYPE_MACRO
//the same applies to the cpp file. the hpp and cpp files creates can be used as if they were not generic. MDPL_GENERIC_TYPE_MACRO is the type the list will store. 
//MDPL_GENERIC_TYPE_NAME_MACRO is the namesapce the specialisation will be in. For clarity it is recomended that this name be associated with the type. If the instructions are followed the
//code should compile with out error. If you get an something undefined error make sure that the two macros are defined before including both the hpp and cpp for a total of 4 hash defines.

//unless otherwise stated all instance of BasicList** refer to a pointer to a pointer and not an array. This is done as so that a function can return a pointer to the list.

//positional insert and remove have intentionally not been included. This is to allow reference stability.

namespace mdpl
{
    namespace standardLibrary
    {
        namespace BasicList
        {
            namespace MDPL_GENERIC_TYPE_NAME_MACRO
            {
                struct BasicListHeader
                {
                    size_t size;
                    size_t capacity;
                };

                struct BasicList
                {
                    BasicListHeader header;
                    MDPL_GENERIC_TYPE_MACRO data[1];
                };

                int constructor(BasicList** list);
                int destructor(BasicList* list);

                int front(const BasicList* list, MDPL_GENERIC_TYPE_MACRO* dst);
                int back(const BasicList* list, MDPL_GENERIC_TYPE_MACRO* dst);
                int get(const BasicList* list, const size_t& pos, MDPL_GENERIC_TYPE_MACRO* dst);
                
                int set(BasicList* list, const size_t& pos, const MDPL_GENERIC_TYPE_MACRO& val);

                bool isEmpty(const BasicList* const list);
                size_t size(const BasicList* const list);
                size_t capacity(const BasicList* const list);

                int reserve(BasicList** list, const size_t newCapacity);
                int clear(BasicList* list);

                int append(BasicList** list, const MDPL_GENERIC_TYPE_MACRO& val);
                int prepend(BasicList** list, const MDPL_GENERIC_TYPE_MACRO& val);

                int removeFront(BasicList* list);
                int removeBack(BasicList* list);

                int allocate(BasicList** list, const size_t& capacity);
                int rellocate(BasicList** list, const size_t& capacity, const size_t& offset);
                //returns number of bytes
                size_t runtimeSizeOf(BasicList* list);
                //converts number of elements to number of bytes
                size_t calculateNewSizeOf(size_t size);
                //converts number of bytes to number of bytes
                size_t trueSizeToUsableSize(size_t size);
            }
        }
    }
}

#undef MDPL_GENERIC_TYPE_MACRO
#undef MDPL_GENERIC_TYPE_NAME_MACRO

#endif //BASIC_LIST_HEADER_GUARD