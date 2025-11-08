#ifndef ALLOCATOR_HEADER_GUARD
#define ALLOCATOR_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "allocation_tracker.hpp"

#include <stdint.h>

#define MDPL_RUNTIME_ALLOCATOR_MINIMAL_ALLIGNMENT 4

int MDPL_RTLIB_ALLOCATOR_allocate(void** ptr, size_t* actualAllocated, const size_t n);
int MDPL_RTLIB_ALLOCATOR_allocateAlligned(void** ptr, size_t* actualAllocated, const size_t allignment, const size_t n);
int MDPL_RTLIB_ALLOCATOR_deallocate(void* ptr);

int MDPL_RTLIB_ALLOCATOR_initialiseAllocator();
int MDPL_RTLIB_ALLOCATOR_destroyAllocator();

//used for debugging
bool MDPL_RTLIB_ALLOCATOR_doesAllocatorHaveActiveMemory();

#endif //ALLOCATOR_HEADER_GUARD