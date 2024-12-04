#include "allocator.hpp"

namespace mdpl
{
    namespace runtimeLib
    {
        namespace allocator
        {
            allocationTracker::AllocationTrackerStruct globalAllocationTracker;

            int allocate(void** ptr, const size_t& n)
            {
                allocateAlligned(ptr, 4, n);
                return 0;
            }
            int allocateAlligned(void** ptr, const size_t allignment, const size_t& n)
            {
                *ptr = aligned_alloc(allignment, n);
                if(ptr == nullptr)
                {
                    printf("MDPL runtime error: failed to allocate %lu bytes of memory with %lu byte allignment.\n", n, allignment);
                    return 1;
                }
                allocationTracker::add(&globalAllocationTracker, *ptr);
                return 0;
            }
            int deallocate(void* ptr)
            {
                if(ptr == nullptr)
                {
                    printf("MDPL runtime error: attempted to deallocate nullptr.\n");
                    return 1;
                }
                else if(allocationTracker::contains(&globalAllocationTracker, ptr))
                {
                    allocationTracker::remove(&globalAllocationTracker, ptr);
                    free(ptr);
                    return 0;
                }
                else
                {
                    printf("MDPL runtime error: failed to deallocate memory as no matching allocation exists.\n");
                    return 1;
                }
            }

            int initialiseAllocator()
            {
                MDPL_RETERR(allocationTracker::constructor(&globalAllocationTracker));
                return 0;
            }
            int destroyAllocator()
            {
                allocationTracker::contentsTuple contents = allocationTracker::getContents(&globalAllocationTracker);
                for(size_t i = 0; i < contents.size; i++)
                {
                    if(contents.array[i] != nullptr)
                    {
                        free(contents.array[i]);
                    }
                }
                MDPL_RETERR(allocationTracker::destructor(&globalAllocationTracker));
                
                return 0;
            }
        }
    }
}