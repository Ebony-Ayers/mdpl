#ifndef ALLOCATOR_HEADER_GUARD
#define ALLOCATOR_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "allocation_tracker.hpp"

#include <cstdint>

#define MDPL_RUNTIME_ALLOCATOR_MINIMAL_ALLIGNMENT 4

namespace mdpl
{
    namespace runtimeLib
    {
        namespace allocator
        {
            int allocate(void** ptr, size_t* actualAllocated, const size_t& n);
            int allocateAlligned(void** ptr, size_t* actualAllocated, const size_t allignment, const size_t& n);
            int deallocate(void* ptr);

            int initialiseAllocator();
            int destroyAllocator();

            //used for debugging
            bool doesAllocatorHaveActiveMemory();
        }
    }
}

#endif //ALLOCATOR_HEADER_GUARD