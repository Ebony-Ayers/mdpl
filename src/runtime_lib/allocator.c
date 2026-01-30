#include "allocator.h"

extern MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker;

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_allocate(void** ptr, size_t* actualAllocated, const size_t n)
{
    MDPL_RTLIB_ALLOCATOR_allocateAlligned(ptr, actualAllocated, MDPL_RUNTIME_ALLOCATOR_MINIMAL_ALLIGNMENT, n);
    return nullptr;
}
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_allocateAlligned(void** ptr, size_t* actualAllocated, const size_t allignment, const size_t n)
{
    //round the size up to the nearest multiple of the allignment
    *actualAllocated = ((n / allignment) + 1) * allignment;
    *ptr = aligned_alloc(allignment, *actualAllocated);
    if(ptr == nullptr)
    {
        printf("MDPL runtime error: failed to allocate %lu bytes of memory with %lu byte allignment.\n", n, allignment);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "failed to allocate requested memory");
    }
    MDPL_RTLIB_ALLOCATION_TRACKER_add(&MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker, *ptr);
    return nullptr;
}
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_deallocate(void* ptr)
{
    if(ptr == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "attempted to deallocate nullptr");
    }
    else if(MDPL_RTLIB_ALLOCATION_TRACKER_contains(&MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker, ptr))
    {
        MDPL_RTLIB_ALLOCATION_TRACKER_remove(&MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker, ptr);
        free(ptr);
        return nullptr;
    }
    else
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "failed to deallocate memory as no matching allocation exists");
    }
}

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_initialiseAllocator()
{
    MDPL_RETERR(MDPL_RTLIB_ALLOCATION_TRACKER_constructor(&MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker));
    return nullptr;
}
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_destroyAllocator()
{
    MDPL_RTLIB_ALLOCATION_TRACKER_contentsTuple contents = MDPL_RTLIB_ALLOCATION_TRACKER_getContents(&MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker);
    for(size_t i = 0; i < contents.size; i++)
    {
        if(contents.array[i] != nullptr)
        {
            free(contents.array[i]);
        }
    }
    MDPL_RETERR(MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker));
    
    return nullptr;
}

bool MDPL_RTLIB_ALLOCATOR_doesAllocatorHaveActiveMemory()
{
    return MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker.size != 0;
}