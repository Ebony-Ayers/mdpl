#include "../pch.hpp"
#include<time.h>
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
    
    printf("======================Running randomised test.\n");
    uint64_t* insertedValues = reinterpret_cast<uint64_t*>(malloc(5000 * sizeof(uint64_t)));
    srand(time(nullptr));
    for(size_t i = 0; i < 5000; i++)
    {
        while(1)
        {
            insertedValues[i] = static_cast<uint64_t>(rand());
            if(insertedValues[i] == reinterpret_cast<uint64_t>(nullptr))
            {
                break;
            }
            bool foundMatch = false;
            for(size_t j = 0; j < i; j++)
            {
                if(insertedValues[i] == insertedValues[j])
                {
                    foundMatch = true;
                    break;
                }
            }
            if(!foundMatch)
            {
                break;
            }
        }
    }
    //double check the test does not contain duplicate items
    for(uint64_t* i = insertedValues; i < insertedValues + 5000; i++)
    {
        for(uint64_t* j = insertedValues; j < insertedValues + 5000; j++)
        {
            if(i != j)
            {
                if(*i == *j)
                {
                    printf("Testing error: Test contains duplicates.\n");
                    cleanUpTracker(&tracker);
                    free(insertedValues);
                    return;
                }
            }
        }
    }
    printf("Adding to tracker.\n");
    for(size_t i = 0; i < 5000; i++)
    {
        mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(insertedValues[i]));
    }
    printf("Checking contents of tracker.\n");
    for(size_t i = 0; i < 5000; i++)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(!res)
        {
            printf("Tracker failed fourth test. Did not contain %lu when it should have.\n", insertedValues[i]);
            cleanUpTracker(&tracker);
            return;
        }
    }
    printf("Removing from tracker.\n");
    for(size_t i = 0; i < 5000; i++)
    {
        mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(insertedValues[i])); 
    }
    printf("Checking contents of tracker.\n");
    for(size_t i = 0; i < 5000; i++)
    {
        bool res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(res)
        {
            printf("Tracker failed fourth test. Contain %lu when it should have been removed.\n", insertedValues[i]);
            cleanUpTracker(&tracker);
            return;
        }
    }
    free(insertedValues);
    printf("Passed test 4.\n\n");

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
        retcode = mdpl::runtimeLib::allocator::deallocate(allocatedPointers[i]);
        if(retcode)
        {
            printf("Something went wrong.\n");
            return;
        }
    }

    printf("Deallocating nullptr. Expect an error\n");
    retcode = mdpl::runtimeLib::allocator::deallocate(nullptr);
    if(!retcode)
    {
        printf("Failed test of deallocating nullptr.\n");
    }

   printf("Destroying allocator.\n");
   mdpl::runtimeLib::allocator::destroyAllocator();

   printf("\nRuntime lib passed all tests.\n");
}

int main(int /*argc*/, char** /*argv*/)
{
    testTracker();
    testAllocator();

    return 0;
}