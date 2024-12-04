#ifndef ALLOCATOR_HEADER_GUARD
#define ALLOCATOR_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "allocation_tracker.hpp"

#include <cstdint>

namespace mdpl
{
    namespace runtimeLib
    {
        namespace allocator
        {
            int allocate(void** ptr, const size_t& n);
            int allocateAlligned(void** ptr, const size_t allignment, const size_t& n);
            int deallocate(void* ptr);

            int initialiseAllocator();
            int destroyAllocator();
        }
    }
}

#endif //ALLOCATOR_HEADER_GUARD