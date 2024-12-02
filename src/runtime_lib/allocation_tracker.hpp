#ifndef ALLOCATION_TRACKER_HEADER_GUARD
#define ALLOCATION_TRACKER_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wold-style-cast"
    #pragma GCC diagnostic ignored "-Wsign-conversion"
    #pragma GCC diagnostic ignored "-Wconversion"
#endif
#include "../../vendor/libdivide/libdivide.h"
#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif

#include <cstdint>

namespace mdpl
{
    namespace runtimeLib
    {
        namespace allocationTracker
        {
            struct AllocationTrackerStruct
            {
                void** array;
                size_t size;
                size_t capacity;
                libdivide::libdivide_u64_t divider;
            };

            AllocationTrackerStruct globalAllocationTracker;

            void constructor(AllocationTrackerStruct* tracker, const size_t& initialSize);
            void destructor(AllocationTrackerStruct* tracker);

            void add(AllocationTrackerStruct* tracker, void* ptr);
            void remove(AllocationTrackerStruct* tracker, void* ptr);
            bool contains(AllocationTrackerStruct* tracker, void* ptr);
        }
    }
}

#endif //ALLOCATION_TRACKER_HEADER_GUARD