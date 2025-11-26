#include "basic_list_template.hpp"

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, constructor)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list)
{
    MDPL_RETERR(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, allocate)(list, MDPL_SHORT_LIST_INITIAL_CAPACITY));
    return nullptr;
}
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, destructor)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    MDPL_RETERR(MDPL_RTLIB_ALLOCATOR_deallocate((void*)list));
    return nullptr;
}

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, front)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst)
{
    if(list->header.size == 0)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_OUT_OF_RANGE, "tried to access first element of list with size 0");
    }
    *dst = list->data[0];
    return nullptr;
}
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, back)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst)
{
    if(list->header.size == 0)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_OUT_OF_RANGE, "tried to access last element of list with size 0");
    }
    *dst = list->data[list->header.size - 1];
    return nullptr;
}
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, get)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const uint32_t pos, MDPL_GENERIC_TYPE_MACRO* dst)
{
    if(list->header.size == 0)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_OUT_OF_RANGE, "tried to access first element of list with size 0");
    }
    else if(pos >= list->header.size)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_OUT_OF_RANGE, "tried to access element outside the bounds of the list");
    }
    *dst = list->data[pos];
    return nullptr;
}

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, set)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const uint32_t pos, const MDPL_GENERIC_TYPE_MACRO* val)
{
    if(pos >= list->header.size)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_OUT_OF_RANGE, "tried to access element outside the bounds of the list");
    }
    list->data[pos] = *val;
    return nullptr;
}

bool MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, isEmpty)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list)
{
    return MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, size)(list) == 0;
}
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, size)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list)
{
    return list->header.size;
}
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, capacity)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list)
{
    return list->header.capacity;
}

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, reserve)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const uint32_t newCapacity)
{
    if(newCapacity > (*list)->header.size)
    {
        MDPL_RETERR(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, rellocate)(list, newCapacity, 0));
    }
    return nullptr;
}
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, clear)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    list->header.size = 0;
    return nullptr;
}

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, append)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO* val)
{
    if((*list)->header.size >= (*list)->header.capacity) [[unlikely]]
    {
        MDPL_RETERR(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, rellocate)(list, (*list)->header.capacity * 2, 0));
    }
    //printf("appending at position %lu with size %lu and capacity %lu.\n", (*list)->header.size, (*list)->header.size, (*list)->header.capacity);
    (*list)->data[(*list)->header.size] = *val;
    (*list)->header.size++;
    return nullptr;
}
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, prepend)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO* val)
{
    if((*list)->header.size >= (*list)->header.capacity) [[unlikely]]
    {
        MDPL_RETERR(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, rellocate)(list, ((*list)->header.capacity * 2), 1));
    }
    else
    {
        memcpy((*list)->data + 1, (*list)->data, ((*list)->header.size) * sizeof(MDPL_GENERIC_TYPE_MACRO));
    }
    (*list)->data[0] = *val;
    (*list)->header.size++;
    return nullptr;
}

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, removeFront)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    memcpy(list->data, list->data + 1, (list->header.size - 1) * sizeof(MDPL_GENERIC_TYPE_MACRO));
    list->header.size--;
    return nullptr;
}
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, removeBack)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    list->header.size--;
    return nullptr;
}

MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, allocate)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const uint32_t capacity)
{
    uint32_t trueCapacity;
    MDPL_RETERR(MDPL_RTLIB_ALLOCATOR_allocateAlligned((void**)list, (size_t*)&trueCapacity, 64, MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, calculateNewSizeOf)((size_t)capacity)));
    (*list)->header.size = 0;
    (*list)->header.capacity = MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, trueSizeToUsableSize)(trueCapacity) / sizeof(MDPL_GENERIC_TYPE_MACRO);
    return nullptr;
}
MDPL_ERROR_Error* MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, rellocate)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const uint32_t capacity, const uint32_t offset)
{
    if(capacity < (*list)->header.size)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "requested reallocation is smaller than existing data");
    }
    MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* pNewList;
    MDPL_RETERR(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, allocate)(&pNewList, capacity));
    pNewList->header.size = (*list)->header.size;
    memcpy(&(pNewList->data[offset]), &((*list)->data), (*list)->header.size * sizeof(MDPL_GENERIC_TYPE_MACRO));
    MDPL_RETERR(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, destructor)(*list));
    *list = pNewList;
    return nullptr;
}

//returns number of bytes
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, runtimeSizeOf)(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    return (uint32_t)sizeof(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)) + (list->header.capacity * (uint32_t)sizeof(MDPL_GENERIC_TYPE_MACRO));
}
//converts number of elements to number of bytes
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, calculateNewSizeOf)(uint32_t size)
{
    return (uint32_t)sizeof(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)) + (size * (uint32_t)sizeof(MDPL_GENERIC_TYPE_MACRO));
}
//converts number of bytes to number of bytes
uint32_t MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, trueSizeToUsableSize)(uint32_t size)
{
    return size - (uint32_t)sizeof(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList));
}

#undef MDPL_GENERIC_TYPE_MACRO
#undef MDPL_GENERIC_TYPE_NAME_MACRO