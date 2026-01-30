#ifndef ALLOCATOR_HEADER_GUARD
#define ALLOCATOR_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#include "allocation_tracker.h"

#include <stdint.h>

#define MDPL_RUNTIME_ALLOCATOR_MINIMAL_ALLIGNMENT 4

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_allocate(void** ptr, size_t* actualAllocated, const size_t n);
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_allocateAlligned(void** ptr, size_t* actualAllocated, const size_t allignment, const size_t n);
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_deallocate(void* ptr);

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_initialiseAllocator();
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATOR_destroyAllocator();

//used for debugging
bool MDPL_RTLIB_ALLOCATOR_doesAllocatorHaveActiveMemory();

#endif //ALLOCATOR_HEADER_GUARD