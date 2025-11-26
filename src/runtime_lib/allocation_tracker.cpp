#include "allocation_tracker.hpp"

MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct MDPL_RTLIB_ALLOCATION_TRACKER_globalAllocationTracker;

//currently this allocator is limited to approximatly 7.5 quintillion allocations.
//this would approximatly half that which is possibe with raw 64 bit addressing.
//I view this as an adequit limitation as no sensible code should ever reach this.
const size_t tablePrimes[] = {101, 1009, 10007, 100003, 1000003, 10000019, 100000007, 1000000007, 10000000019, 100000000003, 1000000000039, 10000000000037, 100000000000031, 1000000000000037, 10000000000000061, 100000000000000003, 1000000000000000003, 10000000000000000051};
const size_t tablePrimesSize = sizeof(tablePrimes) / sizeof(size_t);

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_constructor(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker)
{
    MDPL_RETERR(MDPL_RTLIB_ALLOCATION_TRACKER_constructorWithCapacity(tracker, 0));
    return nullptr;
}
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_destructor(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker)
{
    free(tracker->array);

    return nullptr;
}

MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_add(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr)
{
    //just some fun memory safety things
    if(tracker == nullptr) [[unlikely]]
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tracker is null");
    }
    
    if(ptr == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "attempting to add nullptr to the tracker");
    }
    
    //check if reallocation is required. Reallocation load factor is 75%.
    if(tracker->size > 3 * (tablePrimes[tracker->capacityIndxex] / 4))
    {
        //check for max table size
        if(tracker->capacityIndxex == tablePrimesSize - 1)
        {
            return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_MAXIMUM_SIZE_EXCEDED, "maximum number of tracked allocations exceded. Note that a data strcture containing n elements only requires one alloication. Consider checking your code for infinite loops. If seeing this error message is not the result of a bug, on behalf of the MDPL comunity, congratulations");
        }
        
        MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct newTracker;
        MDPL_RTLIB_ALLOCATION_TRACKER_constructorWithCapacity(&newTracker, tracker->capacityIndxex + 1);
        for(size_t i = 0; i < tablePrimes[tracker->capacityIndxex]; i++)
        {
            if(tracker->array[i] != nullptr)
            {
                MDPL_RETERR(MDPL_RTLIB_ALLOCATION_TRACKER_addNoReallocationCheck(&newTracker, tracker->array[i]));
            }
        }
        MDPL_RETERR(MDPL_RTLIB_ALLOCATION_TRACKER_addNoReallocationCheck(&newTracker, ptr));

        //clean up old tracker
        MDPL_RTLIB_ALLOCATION_TRACKER_destructor(tracker);

        //copy the new tracker to the old one
        tracker->array = newTracker.array;
        tracker->size = newTracker.size;
        tracker->capacityIndxex = newTracker.capacityIndxex;
        tracker->divider = newTracker.divider;

        return nullptr;
    }
    else
    {
        MDPL_RETERR(MDPL_RTLIB_ALLOCATION_TRACKER_addNoReallocationCheck(tracker, ptr));
        return nullptr;
    }
}
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_remove(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr)
{
    const size_t index = MDPL_RTLIB_ALLOCATION_TRACKER_indexOf(tracker, ptr);
    if(index != tablePrimes[tracker->capacityIndxex])
    {
        tracker->array[index] = nullptr;
        tracker->size--;
        return nullptr;
    }
    else
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "attempted to remove pointer not in tracker from tracker");
    }
}
bool MDPL_RTLIB_ALLOCATION_TRACKER_contains(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr)
{
    return tracker->array[MDPL_RTLIB_ALLOCATION_TRACKER_indexOf(tracker, ptr)] == ptr;
}

MDPL_RTLIB_ALLOCATION_TRACKER_contentsTuple MDPL_RTLIB_ALLOCATION_TRACKER_getContents(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker)
{
    MDPL_RTLIB_ALLOCATION_TRACKER_contentsTuple tuple;
    tuple.array = tracker->array;
    tuple.size = tracker->size;
    return tuple;
}

size_t MDPL_RTLIB_ALLOCATION_TRACKER_indexOf(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr)
{
    //compute ptr mod capacity
    const uint64_t val = (uint64_t)ptr;
    const size_t divResult = libdivide_u64_do(val, &(tracker->divider));
    const size_t capacity = tablePrimes[tracker->capacityIndxex];
    size_t index = (size_t)val - (divResult * capacity);
    
    //linear probe
    size_t counter = 0;
    while((tracker->array[index] != ptr) && (counter < capacity))
    {
        index++;
        //the compiler should make this branchless which will avoid a mod
        if(index >= capacity) { index = 0; }
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
void MDPL_RTLIB_ALLOCATION_TRACKER_initialiseArray(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, size_t allocationSize)
{
    if((uint64_t)nullptr < 256)
    {
        memset(tracker->array, (int)((uint64_t)nullptr), allocationSize);
    }
    else
    {
        for(size_t i = 0; i < allocationSize; i++)
        {
            tracker->array[i] = nullptr;
        }
    }
}
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_addNoReallocationCheck(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, void* ptr)
{
    //just some fun memory safety things
    if(tracker == nullptr) [[unlikely]]
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tracker is null");
    }
    if(tracker->array == nullptr) [[unlikely]]
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tracker->array is null");
    }

    //compute ptr mod capacity
    const uint64_t val = (uint64_t)ptr;
    const size_t divResult = libdivide_u64_do(val, &(tracker->divider));
    const size_t capacity = tablePrimes[tracker->capacityIndxex];
    size_t index = (size_t)val - (divResult * capacity);
    
    //linear probe
    size_t counter = 0;
    while((tracker->array[index] != ptr) && (counter < capacity) && (tracker->array[index] != nullptr))
    {
        index++;
        //the compiler should make this branchless which will avoid a mod
        if(index >= capacity) { index = 0; }
        counter++;
    }

    if(index >= capacity)
    {
        printf("Tracker out of bounds.\n");
    }

    //we have three cases, found pointer, found empty space, exceded size of container
    if(tracker->array[index] == ptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "attempted to add pointer already in tracker to tracker");
    }
    else if(counter == capacity)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "attempted to add pointer to full tracker. Please submit a bug report as this should be impossible");
    }
    else
    {
        tracker->array[index] = ptr;
        tracker->size++;
        return nullptr;
    }
}
MDPL_ERROR_Error* MDPL_RTLIB_ALLOCATION_TRACKER_constructorWithCapacity(MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct* tracker, size_t initialCapaityIndex)
{
    tracker->size = 0;
    tracker->capacityIndxex = initialCapaityIndex;
    const size_t capacity = tablePrimes[tracker->capacityIndxex];
    tracker->divider = libdivide_u64_gen(capacity);
    
    const size_t allocationSize = capacity * sizeof(void*);
    tracker->array = (void**)malloc(allocationSize);
    if(tracker->array == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "could not allocate enough memory for allocation tracker");
    }
    //initialise the array. This is done here rather than in a function do make valgrind stop complaining
    for(size_t i = 0; i < capacity; i++)
    {
        tracker->array[i] = nullptr;
    }
    //initialiseArray(tracker, capacity);
    return nullptr;
}