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
            void* allocate(const size_t& n);
            void* allocate_alligned(const size_t allignment, const size_t& n);
            void deallocate(void* ptr);
        }
    }
}

#endif //ALLOCATOR_HEADER_GUARD