#ifndef ALLOCATION_TRACKER_HEADER_GUARD
#define ALLOCATION_TRACKER_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
    #pragma GCC diagnostic ignored "-Wconversion"
#endif
#include "../../vendor/libdivide/libdivide.h"
#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif

//MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct
typedef struct
{
    void** array;
    size_t size;
    size_t capacityIndxex;
    struct libdivide_u64_t divider;
} MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct;

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_constructor(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker);
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_destructor(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker);

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_add(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr);
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_remove(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr);
bool MDPL_RTLIB_ALLOCATION_TRACKER_contains(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr);

//MDPL_RTLIB_ALLOCATION_TRACKER_contentsTuple
typedef struct
{
    void** array;
    size_t size;
} MDPL_RTLIB_ALLOCATION_TRACKER_contentsTuple;
MDPL_RTLIB_ALLOCATION_TRACKER_contentsTuple MDPL_RTLIB_ALLOCATION_TRACKER_getContents(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker);

size_t MDPL_RTLIB_ALLOCATION_TRACKER_indexOf(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr);
void MDPL_RTLIB_ALLOCATION_TRACKER_initialiseArray(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, size_t allocationSize);
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_addNoReallocationCheck(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr);
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_constructorWithCapacity(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, size_t initialCapaityIndex);

#endif //ALLOCATION_TRACKER_HEADER_GUARD