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
                size_t capacityIndxex;
                libdivide::libdivide_u64_t divider;
            };

            int constructor(AllocationTrackerStruct* tracker);
            int destructor(AllocationTrackerStruct* tracker);

            int add(AllocationTrackerStruct* tracker, void* ptr);
            int remove(AllocationTrackerStruct* tracker, void* ptr);
            bool contains(AllocationTrackerStruct* tracker, void* ptr);

            struct contentsTuple
            {
                void** array;
                size_t size;
            };
            contentsTuple getContents(AllocationTrackerStruct* tracker);

            size_t indexOf(AllocationTrackerStruct* tracker, void* ptr);
            void initialiseArray(AllocationTrackerStruct* tracker, size_t allocationSize);
            int addNoReallocationCheck(AllocationTrackerStruct* tracker, void* ptr);
            int constructorWithCapacity(AllocationTrackerStruct* tracker, size_t initialCapaityIndex);
        }
    }
}

#endif //ALLOCATION_TRACKER_HEADER_GUARD