#include "allocation_tracker.hpp"

namespace mdpl
{
    namespace runtimeLib
    {
        namespace allocationTracker
        {
            AllocationTrackerStruct globalAllocationTracker;

            //current this allocator is limited to approximatly 7,500,000,000,000 allocations.
            //this would limit you to 30TB of int32 if you stored one int32 per data structure.
            //I view this as an adequit limitation as no sensible code should ever reach this.
            const size_t tablePrimes[] = {101, 1009, 10007, 100003, 1000003, 10000019, 100000007, 1000000007, 10000000019, 100000000003, 1000000000039};
            const size_t tablePrimesSize = sizeof(tablePrimes) / sizeof(size_t);

            int constructor(AllocationTrackerStruct* tracker)
            {
                MDPL_RETERR(constructorWithCapacity(tracker, 0));
                return 0;
            }
            int destructor(AllocationTrackerStruct* tracker)
            {
                free(tracker->array);

                return 0;
            }

            int add(AllocationTrackerStruct* tracker, void* ptr)
            {
                if(ptr == nullptr)
                {
                    printf("MDPL runtime error: attempting to add nullptr to the tracker.\n");
                    return 1;
                }
                
                //check if reallocation is required. Reallocation load factor is 75%.
                if(tracker->size > 3 * (tablePrimes[tracker->capacityIndxex] / 4))
                {
                    //check for max table size
                    if(tracker->capacityIndxex == tablePrimesSize - 1)
                    {
                        printf("MDPL runtime error: maximum number of tracked allocations exceded. MDPL does not allow more than %lu allocations. Note that a data strcture containing n elements only requires one alloication. Consider checking your code for infinite loops. If seeing this error message is not the result of a bug, on behalf of the MDPL comunity, congratulations.\n", tracker->size);
                        return 1;
                    }
                    
                    AllocationTrackerStruct newTracker;
                    constructorWithCapacity(&newTracker, tracker->capacityIndxex + 1);
                    for(size_t i = 0; i < tablePrimes[tracker->capacityIndxex]; i++)
                    {
                        if(tracker->array[i] != nullptr)
                        {
                            MDPL_RETERR(addNoReallocationCheck(&newTracker, tracker->array[i]));
                        }
                    }
                    MDPL_RETERR(addNoReallocationCheck(&newTracker, ptr));

                    //clean up old tracker
                    destructor(tracker);

                    //copy the new tracker to the old one
                    tracker->array = newTracker.array;
                    tracker->size = newTracker.size;
                    tracker->capacityIndxex = newTracker.capacityIndxex;
                    tracker->divider = newTracker.divider;

                    return 0;
                }
                else
                {
                    MDPL_RETERR(addNoReallocationCheck(tracker, ptr));
                    return 0;
                }
            }
            int remove(AllocationTrackerStruct* tracker, void* ptr)
            {
                const size_t index = indexOf(tracker, ptr);
                if(index != tablePrimes[tracker->capacityIndxex])
                {
                    tracker->array[index] = nullptr;
                    tracker->size--;
                    return 0;
                }
                else
                {
                    printf("MDPL runtime error: attempted to remove pointer not in tracker from tracker.\n");
                    return 1;
                }
            }
            bool contains(AllocationTrackerStruct* tracker, void* ptr)
            {
                return indexOf(tracker, ptr) != tablePrimes[tracker->capacityIndxex];
            }

            contentsTuple getContents(AllocationTrackerStruct* tracker)
            {
                contentsTuple tuple;
                tuple.array = tracker->array;
                tuple.size = tracker->size;
                return tuple;
            }

            size_t indexOf(AllocationTrackerStruct* tracker, void* ptr)
            {
                //compute ptr mod capacity
                const uint64_t val = reinterpret_cast<uint64_t>(ptr);
                const size_t divResult = static_cast<size_t>(libdivide::libdivide_u64_do(val, &(tracker->divider)));
                const size_t capacity = tablePrimes[tracker->capacityIndxex];
                size_t index = static_cast<size_t>(val) - (divResult * capacity);
                
                //linear probe
                size_t counter = 0;
                while((tracker->array[index] != ptr) && (counter < capacity))
                {
                    index++;
                    counter++;
                }

                //determine if we have found the ptr
                //if we have not found return capacity as it is the smallest nonvalid index
                if(tracker->array[index] == ptr)
                {
                    return index;
                }
                else
                {
                    return capacity;
                }
            }
            void initialiseArray(AllocationTrackerStruct* tracker, size_t allocationSize)
            {
                if(reinterpret_cast<uint64_t>(nullptr) < 256)
                {
                    memset(tracker->array, static_cast<int>(reinterpret_cast<uint64_t>(nullptr)), allocationSize);
                }
                else
                {
                    for(size_t i = 0; i < allocationSize; i++)
                    {
                        tracker->array[i] = nullptr;
                    }
                }
            }
            int addNoReallocationCheck(AllocationTrackerStruct* tracker, void* ptr)\
            {
                //compute ptr mod capacity
                const uint64_t val = reinterpret_cast<uint64_t>(ptr);
                const size_t divResult = static_cast<size_t>(libdivide::libdivide_u64_do(val, &(tracker->divider)));
                const size_t capacity = tablePrimes[tracker->capacityIndxex];
                size_t index = static_cast<size_t>(val) - (divResult * capacity);
                
                //linear probe
                size_t counter = 0;
                while((tracker->array[index] != ptr) && (counter < capacity) && (tracker->array[index] != nullptr))
                {
                    index++;
                    counter++;
                }

                //we have three cases, found pointer, found empty space, exceded size of container
                if(tracker->array[index] == ptr)
                {
                    printf("MDPL runtime error: attempted to add pointer already in tracker to tracker.\n");
                    return 1;
                }
                else if(counter == capacity)
                {
                    printf("MDPL runtime error: attempted to add pointer to full tracker. Please submit a bug report as this should be impossible.\n");
                    return 1;
                }
                else
                {
                    tracker->array[index] = ptr;
                    tracker->size++;
                    return 0;
                }
            }
            int constructorWithCapacity(AllocationTrackerStruct* tracker, size_t initialCapaityIndex)
            {
                tracker->size = 0;
                tracker->capacityIndxex = initialCapaityIndex;
                tracker->divider = libdivide::libdivide_u64_gen(tablePrimes[tracker->capacityIndxex]);
                
                const size_t allocationSize = tablePrimes[tracker->capacityIndxex] * sizeof(void*);
                tracker->array = reinterpret_cast<void**>(malloc(allocationSize));
                if(tracker->array == nullptr)
                {
                    printf("MDPL runtime error: could not allocate enough memory for allocation tracker. Attempted to allocate %lu byhtes.\n", allocationSize);
                    return 1;
                }
                initialiseArray(tracker, allocationSize);
                return 0;
            }
        }
    }
}