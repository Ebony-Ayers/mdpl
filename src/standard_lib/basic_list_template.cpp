#include "basic_list_template.hpp"

int constructor(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list)
{
    MDPL_RETERR(allocate(list, MDPL_SHORT_LIST_INITIAL_CAPACITY));
    return 0;
}
int destructor(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    MDPL_RETERR(mdpl::runtimeLib::allocator::deallocate((void*)list));
    return 0;
}

int MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, front)(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst)
{
    if(list->header.size == 0)
    {
        printf("Out of bounds error: tried to access first element of list with size 0.\n");
        return 1;
    }
    *dst = list->data[0];
    return 0;
}
int back(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, MDPL_GENERIC_TYPE_MACRO* dst)
{
    if(list->header.size == 0)
    {
        printf("Out of bounds error: tried to access last element of list with size 0.\n");
        return 1;
    }
    *dst = list->data[list->header.size - 1];
    return 0;
}
int get(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const size_t& pos, MDPL_GENERIC_TYPE_MACRO* dst)
{
    if(list->header.size == 0)
    {
        printf("Out of bounds error: tried to access first element of list with size 0.\n");
        return 1;
    }
    else if(pos >= list->header.size)
    {
        printf("Out of bounds error: tried to access element outside the bounds of the list.\n");
        return 1;
    }
    *dst = list->data[pos];
    return 0;
}

int set(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list, const size_t& pos, const MDPL_GENERIC_TYPE_MACRO& val)
{
    if(pos >= list->header.size)
    {
        printf("Out of bounds error: tried to access element outside the bounds of the list.\n");
        return 1;
    }
    list->data[pos] = val;
    return 0;
}

bool isEmpty(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list)
{
    return size(list) == 0;
}
size_t size(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list)
{
    return list->header.size;
}
size_t capacity(const MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* const list)
{
    return list->header.capacity;
}

int reserve(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const size_t newCapacity)
{
    if(newCapacity > (*list)->header.size)
    {
        MDPL_RETERR(rellocate(list, newCapacity, 0));
    }
    return 0;
}
int clear(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    list->header.size = 0;
    return 0;
}

int append(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO& val)
{
    if((*list)->header.size >= (*list)->header.capacity) [[unlikely]]
    {
        MDPL_RETERR(rellocate(list, (*list)->header.capacity * 2, 0));
    }
    //printf("appending at position %lu with size %lu and capacity %lu.\n", (*list)->header.size, (*list)->header.size, (*list)->header.capacity);
    (*list)->data[(*list)->header.size] = val;
    (*list)->header.size++;
    return 0;
}
int prepend(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const MDPL_GENERIC_TYPE_MACRO& val)
{
    if((*list)->header.size >= (*list)->header.capacity) [[unlikely]]
    {
        MDPL_RETERR(rellocate(list, ((*list)->header.capacity * 2), 1));
    }
    else
    {
        memcpy((*list)->data + 1, (*list)->data, ((*list)->header.size) * sizeof(MDPL_GENERIC_TYPE_MACRO));
    }
    (*list)->data[0] = val;
    (*list)->header.size++;
    return 0;
}

int removeFront(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    memcpy(list->data, list->data + 1, (list->header.size - 1) * sizeof(MDPL_GENERIC_TYPE_MACRO));
    list->header.size--;
    return 0;
}
int removeBack(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    list->header.size--;
    return 0;
}

int allocate(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const size_t& capacity)
{
    size_t trueCapacity;
    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned((void**)list, &trueCapacity, 64, calculateNewSizeOf(capacity)));
    (*list)->header.size = 0;
    (*list)->header.capacity = trueSizeToUsableSize(trueCapacity) / sizeof(MDPL_GENERIC_TYPE_MACRO);
    return 0;
}
int rellocate(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)** list, const size_t& capacity, const size_t& offset)
{
    MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* pNewList;
    MDPL_RETERR(allocate(&pNewList, capacity));
    pNewList->header.size = (*list)->header.size;
    memcpy(&(pNewList->data[offset]), &((*list)->data), (*list)->header.size * sizeof(MDPL_GENERIC_TYPE_MACRO));
    MDPL_RETERR(destructor(*list));
    *list = pNewList;
    return 0;
}

//returns number of bytes
size_t runtimeSizeOf(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)* list)
{
    return sizeof(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)) + (list->header.capacity * sizeof(MDPL_GENERIC_TYPE_MACRO));
}
//converts number of elements to number of bytes
size_t calculateNewSizeOf(size_t size)
{
    return sizeof(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList)) + (size * sizeof(MDPL_GENERIC_TYPE_MACRO));
}
//converts number of bytes to number of bytes
size_t trueSizeToUsableSize(size_t size)
{
    return size - sizeof(MDPL_CONCAT(MDPL_STDLIB_SHORT_LIST, MDPL_GENERIC_TYPE_MACRO, ShortList));
}

#undef MDPL_GENERIC_TYPE_MACRO
#undef MDPL_GENERIC_TYPE_NAME_MACRO