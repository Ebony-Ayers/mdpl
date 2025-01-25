#include "allocator.hpp"

namespace mdpl
{
    namespace runtimeLib
    {
        namespace allocator
        {
            allocationTracker::AllocationTrackerStruct globalAllocationTracker;

            int allocate(void** ptr, size_t* actualAllocated, const size_t& n)
            {
                allocateAlligned(ptr, actualAllocated, MDPL_RUNTIME_ALLOCATOR_MINIMAL_ALLIGNMENT, n);
                return 0;
            }
            int allocateAlligned(void** ptr, size_t* actualAllocated, const size_t allignment, const size_t& n)
            {
                //round the size up to the nearest multiple of the allignment
                *actualAllocated = ((n / allignment) + 1) * allignment;
                *ptr = aligned_alloc(allignment, *actualAllocated);
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

            bool doesAllocatorHaveActiveMemory()
            {
                return globalAllocationTracker.size != 0;
            }
        }
    }
}