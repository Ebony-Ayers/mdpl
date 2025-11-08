#include "../pch.hpp"
#include<time.h>
#include "../common/mdpl_common.hpp"
#include "allocator.hpp"

void testTracker()
{
    printf("Testing allocation tracker.\n");

    MDPL_RTLIB_ALLOCATION_TRACKER_AllocationTrackerStruct tracker;

    int retcode;
    bool res;

    //test 1: construct tracker
    retcode = MDPL_RTLIB_ALLOCATION_TRACKER_constructor(&tracker);
    if(retcode)
    {
        printf("failed test 1: error during constructor.\n");
        return;
    }

    //test 2: add without reallocation
    for(uint64_t i = 1000; i < 1050; i++)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_add(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 2: error during add.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 1050; i++)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("failed test 2: Did not contain %lu when it should have.\n", i);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 1050; i++)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_remove(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 2: error during remove.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 1050; i++)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("failed test 2: Contains %lu when it should have.\n", i);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }

    //test 3: add without reallocation
    for(uint64_t i = 1000; i < 2000; i++)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_add(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 3: error during add.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 2000; i++)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("failed test 3: Did not contain %lu when it should have.\n", i);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 2000; i++)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_remove(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 3: error during remove.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1000; i < 2000; i++)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("failed test 3: Contains %lu when it should have.\n", i);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }

    //test 4: add non-consecuative values
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_add(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 4: error during add.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(i));
        if(!res)
        {
            printf("failed test 4: Did not contain %lu when it should have.\n", i);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_remove(&tracker, reinterpret_cast<void*>(i));
        if(retcode)
        {
            printf("failed test 4: error during remove.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(uint64_t i = 1109; i < 113996; i+=113)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(i));
        if(res)
        {
            printf("failed test 4: Contains %lu when it should have.\n", i);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
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
                    MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
                    free(insertedValues);
                    return;
                }
            }
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_add(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(retcode)
        {
            printf("failed test 5: error during add.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(!res)
        {
            printf("failed test 5: Did not contain %lu when it should have.\n", insertedValues[i]);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        retcode = MDPL_RTLIB_ALLOCATION_TRACKER_remove(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(retcode)
        {
            printf("failed test 5: error during remove.\n");
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    for(size_t i = 0; i < 5000; i++)
    {
        res = MDPL_RTLIB_ALLOCATION_TRACKER_contains(&tracker, reinterpret_cast<void*>(insertedValues[i]));
        if(res)
        {
            printf("failed test 5: Contains %lu when it should have.\n", insertedValues[i]);
            MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
            return;
        }
    }
    free(insertedValues);

    //test 6: destructor
    retcode = MDPL_RTLIB_ALLOCATION_TRACKER_destructor(&tracker);
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
    retcode = MDPL_RTLIB_ALLOCATOR_initialiseAllocator();
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
        retcode = MDPL_RTLIB_ALLOCATOR_allocate(allocatedPointers + i, &trueCapacity, 8);
        if(retcode)
        {
            printf("failed test 2: Error during allocate.\n");
            MDPL_RTLIB_ALLOCATOR_destroyAllocator();
            return;
        }
        else if(trueCapacity < 8)
        {
            printf("failed test 2: Allocated capacity is less than the requirested capcity.\n");
            MDPL_RTLIB_ALLOCATOR_destroyAllocator();
            return;
        }
    }

    //test 3: deallocating unallocated memory
    void* ptr = malloc(8);
    retcode = MDPL_RTLIB_ALLOCATOR_deallocate(ptr);
    free(ptr);
    if(!retcode)
    {
        printf("failed test 3: Successfully deallocated unallocated memory.\n");
        MDPL_RTLIB_ALLOCATOR_destroyAllocator();
        return;
    }
    
    //test 3: deallocating allocated memory
    for(size_t i = 0; i < 10; i++)
    {
        retcode = MDPL_RTLIB_ALLOCATOR_deallocate(allocatedPointers[i]);
        if(retcode)
        {
            printf("failed test 3: Error during deallocate.\n");
            MDPL_RTLIB_ALLOCATOR_destroyAllocator();
            return;
        }
    }

    //test 4: deallocating nullptr
    retcode = MDPL_RTLIB_ALLOCATOR_deallocate(nullptr);
    if(!retcode)
    {
        printf("failed test 4: Successfully deallocated nullptr.\n");
        MDPL_RTLIB_ALLOCATOR_destroyAllocator();
        return;
    }

    retcode = MDPL_RTLIB_ALLOCATOR_destroyAllocator();
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
