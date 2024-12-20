#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "allocator.hpp"

void cleanUpTracker(mdpl::runtimeLib::allocationTracker::AllocationTrackerStruct* tracker)
{
    printf("Deconstructing tracker.\n");
    mdpl::runtimeLib::allocationTracker::destructor(tracker);
}

void testTracker()
{
    printf("Testing allocation tracker.\n");

    mdpl::runtimeLib::allocationTracker::AllocationTrackerStruct tracker;
    printf("Constructing tracker.\n");
    mdpl::runtimeLib::allocationTracker::constructor(&tracker);

    printf("Checking tracker works with out reallocation.\n");
    printf("Adding to tracker.\n");
    for(uint64_t i = 1000; i < 1050; i++)
    {
        mdpl::runtimeLib::allocationTracker::add(&tracker, reinterpret_cast<void*>(i)); 
    }
    printf("Checking contents of tracker.\n");
    for(uint64_t i = 1000; i < 1050; i++)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("Tracker failed first test. Did not contain %lu when it should have.\n", i);
            cleanUpTracker(&tracker);
            return;
        }
    }
    printf("Removing from tracker.\n");
    for(uint64_t i = 1000; i < 1050; i++)
    {
        mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(i)); 
    }
    printf("Checking contents of tracker.\n");
    for(uint64_t i = 1000; i < 1050; i++)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("Tracker failed first test. Contain %lu when it should have been removed.\n", i);
            cleanUpTracker(&tracker);
            return;
        }
    }
    printf("Passed test 1.\n\n");

    printf("Checking tracker works with reallocation.\n");
    printf("Adding to tracker.\n");
    for(uint64_t i = 1000; i < 2000; i++)
    {
        mdpl::runtimeLib::allocationTracker::add(&tracker, reinterpret_cast<void*>(i)); 
    }
    printf("Checking contents of tracker.\n");
    for(uint64_t i = 1000; i < 2000; i++)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("Tracker failed second test. Did not contain %lu when it should have.\n", i);
            cleanUpTracker(&tracker);
            return;
        }
    }
    printf("Removing from tracker.\n");
    for(uint64_t i = 1000; i < 2000; i++)
    {
        mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(i)); 
    }
    printf("Checking contents of tracker.\n");
    for(uint64_t i = 1000; i < 2000; i++)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("Tracker failed second test. Contain %lu when it should have been removed.\n", i);
            cleanUpTracker(&tracker);
            return;
        }
    }
    printf("Passed test 2.\n\n");

    printf("Checking tracker works with nonconsecutive inputs.\n");
    printf("Adding to tracker.\n");
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        mdpl::runtimeLib::allocationTracker::add(&tracker, reinterpret_cast<void*>(i)); 
    }
    printf("Checking contents of tracker.\n");
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("Tracker failed third test. Did not contain %lu when it should have.\n", i);
            cleanUpTracker(&tracker);
            return;
        }
    }
    printf("Removing from tracker.\n");
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(i)); 
    }
    printf("Checking contents of tracker.\n");
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("Tracker failed third test. Contain %lu when it should have been removed.\n", i);
            cleanUpTracker(&tracker);
            return;
        }
    }
    printf("Passed test 3.\n\n");

    cleanUpTracker(&tracker);
}

void testAllocator()
{
    printf("\nTesting allocator.\n");

    printf("Initialising allocator,\n");
    mdpl::runtimeLib::allocator::initialiseAllocator();

    printf("Allocating memory.\n");
    void* allocatedPointers[10];
    for(size_t i = 0; i < 10; i++)
    {
        size_t trueCapacity;
        int retcode = mdpl::runtimeLib::allocator::allocate(allocatedPointers + i, &trueCapacity, 8);
        if(retcode)
        {
            printf("Something went wrong.\n");
            return;
        }
        else if(trueCapacity < 8)
        {
            printf("Allocated capacity is less than the requirested capcity.\n");
            return;
        }
    }

    printf("Deallocating unallocated memory. Expect a warning.\n");
    void* ptr = malloc(8);
    int retcode = mdpl::runtimeLib::allocator::deallocate(ptr);
    free(ptr);
    if(!retcode)
    {
        printf("Failed test of deallocating unallocated memory.\n");
    }
    
    printf("Deallocating memeory.\n");
    for(size_t i = 0; i < 10; i++)
    {
        int retcode = mdpl::runtimeLib::allocator::deallocate(allocatedPointers[i]);
        if(retcode)
        {
            printf("Something went wrong.\n");
            return;
        }
    }

    printf("Deallocating pullptr. Expect an error\n");
    retcode = mdpl::runtimeLib::allocator::deallocate(nullptr);
    if(!retcode)
    {
        printf("Failed test of deallocating nullptr.\n");
    }

   printf("Destroying allocator.\n");
   mdpl::runtimeLib::allocator::destroyAllocator();
}

int main(int argc, char** argv)
{
    testTracker();
    testAllocator();

    return 0;
}