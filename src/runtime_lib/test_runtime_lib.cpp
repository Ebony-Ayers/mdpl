#include "../pch.hpp"
#include<time.h>
#include "../common/mdpl_common.hpp"
#include "allocator.hpp"

void testTracker()
{
    printf("Testing allocation tracker.\n");

    mdpl::runtimeLib::allocationTracker::AllocationTrackerStruct tracker;

    int retcode;
    bool res;

    //test 1: construct tracker
    retcode = mdpl::runtimeLib::allocationTracker::constructor(&tracker);
    if(retcode)
    {
        printf("failed test 1: error during constructor.\n");
        return;
    }

    //test 2: add without reallocation
    for(uint64_t i = 1000; i < 1050; i++)
    {
        retcode = mdpl::runtimeLib::allocationTracker::add(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 2: error during add.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 1050; i++)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("failed test 2: Did not contain %lu when it should have.\n", i);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 1050; i++)
    {
        retcode = mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 2: error during remove.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 1050; i++)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("failed test 2: Contains %lu when it should have.\n", i);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }

    //test 3: add without reallocation
    for(uint64_t i = 1000; i < 2000; i++)
    {
        retcode = mdpl::runtimeLib::allocationTracker::add(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 3: error during add.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 2000; i++)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("failed test 3: Did not contain %lu when it should have.\n", i);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 2000; i++)
    {
        retcode = mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 3: error during remove.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 2000; i++)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("failed test 3: Contains %lu when it should have.\n", i);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }

    //test 4: add non-consecuative values
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        retcode = mdpl::runtimeLib::allocationTracker::add(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 4: error during add.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("failed test 4: Did not contain %lu when it should have.\n", i);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        retcode = mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 4: error during remove.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("failed test 4: Contains %lu when it should have.\n", i);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    
    //test 5: randomised values
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
                    printf("failed test 5: Randomixed numbers contains duplicates.\n");
                    mdpl::runtimeLib::allocationTracker::destructor(&tracker);
                    free(insertedValues);
                    return;
                }
            }
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        retcode = mdpl::runtimeLib::allocationTracker::add(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(retcode)
        {
            printf("failed test 5: error during add.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(!res)
        {
            printf("failed test 5: Did not contain %lu when it should have.\n", insertedValues[i]);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        retcode = mdpl::runtimeLib::allocationTracker::remove(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(retcode)
        {
            printf("failed test 5: error during remove.\n");
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        res = mdpl::runtimeLib::allocationTracker::contains(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(res)
        {
            printf("failed test 5: Contains %lu when it should have.\n", insertedValues[i]);
            mdpl::runtimeLib::allocationTracker::destructor(&tracker);
            return;
        }
    }
    free(insertedValues);

    //test 6: destructor
    retcode = mdpl::runtimeLib::allocationTracker::destructor(&tracker);
    if(retcode)
    {
        printf("failed test 6: error during destructor.\n");
        return;
    }

    printf("Allocation tracker passed all tests.\n");
}

void testAllocator()
{
    int retcode;

    //test 1: initialiseAllocator
    retcode = mdpl::runtimeLib::allocator::initialiseAllocator();
    if(retcode)
    {
        printf("failed test 1: Error during initialiseAllocator.\n");
        return;
    }

    //test 2: allocating memory
    void* allocatedPointers[10];
    for(size_t i = 0; i < 10; i++)
    {
        size_t trueCapacity;
        retcode = mdpl::runtimeLib::allocator::allocate(allocatedPointers + i, &trueCapacity, 8);
        if(retcode)
        {
            printf("failed test 2: Error during allocate.\n");
            mdpl::runtimeLib::allocator::destroyAllocator();
            return;
        }
        else if(trueCapacity < 8)
        {
            printf("failed test 2: Allocated capacity is less than the requirested capcity.\n");
            mdpl::runtimeLib::allocator::destroyAllocator();
            return;
        }
    }

    //test 3: deallocating unallocated memory
    void* ptr = malloc(8);
    retcode = mdpl::runtimeLib::allocator::deallocate(ptr);
    free(ptr);
    if(!retcode)
    {
        printf("failed test 3: Successfully deallocated unallocated memory.\n");
        mdpl::runtimeLib::allocator::destroyAllocator();
        return;
    }
    
    //test 3: deallocating allocated memory
    for(size_t i = 0; i < 10; i++)
    {
        retcode = mdpl::runtimeLib::allocator::deallocate(allocatedPointers[i]);
        if(retcode)
        {
            printf("failed test 3: Error during deallocate.\n");
            mdpl::runtimeLib::allocator::destroyAllocator();
            return;
        }
    }

    //test 4: deallocating nullptr
    retcode = mdpl::runtimeLib::allocator::deallocate(nullptr);
    if(!retcode)
    {
        printf("failed test 4: Successfully deallocated nullptr.\n");
        mdpl::runtimeLib::allocator::destroyAllocator();
        return;
    }

    retcode = mdpl::runtimeLib::allocator::destroyAllocator();
    if(retcode)
    {
        printf("failed test 4: Error during destroyAllocator.\n");
        return;
    }

    printf("Allocator passed all tests.\n");
}

int main(int /*argc*/, char** /*argv*/)
{
    testTracker();
    testAllocator();

    return 0;
}
