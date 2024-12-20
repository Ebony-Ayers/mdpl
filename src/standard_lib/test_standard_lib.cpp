#include "../pch.hpp"
#include "../common/mdpl_common.hpp"

#define MDPL_GENERIC_TYPE_MACRO int
#define MDPL_GENERIC_TYPE_NAME_MACRO t_int
#include "basic_list_template.hpp"
#define MDPL_GENERIC_TYPE_MACRO int
#define MDPL_GENERIC_TYPE_NAME_MACRO t_int
#include "basic_list_template.cpp"

//this is not required for use however is needed for debugging
#include "../runtime_lib/allocator.hpp"

void testBasicList()
{
    int retcode;

    //test constructor
    mdpl::standardLibrary::BasicList::t_int::BasicList* pList;
    retcode = mdpl::standardLibrary::BasicList::t_int::constructor(&pList);
    if(retcode)
    {
        printf("Failed to construct basic list.\n");
        return;
    }

    //test newly constructed list is empty
    if(!mdpl::standardLibrary::BasicList::t_int::isEmpty(pList))
    {
        printf("Newly constucted list is not empty.\n");
        return;
    }

    //test that append works with out reallocating
    for(int i = 0; i < 100; i++)
    {
        retcode = mdpl::standardLibrary::BasicList::t_int::append(&pList, i);
        if(retcode)
        {
            printf("Failed to append to basic list.\n");
            return;
        }
    }
    for(int i = 0; i < 100; i++)
    {
        int val;
        retcode = mdpl::standardLibrary::BasicList::t_int::get(pList, i, &val);
        if(retcode)
        {
            printf("Failed to get data from basic list.\n");
            return;
        }
        if(val != i)
        {
            printf("basic list did not contain the right data.\n");
            return;
        }
    }
    //test that append works with reallocating
    for(int i = 100; i < 1000; i++)
    {
        retcode = mdpl::standardLibrary::BasicList::t_int::append(&pList, static_cast<size_t>(i));
        if(retcode)
        {
            printf("Failed to append to basic list.\n");
            return;
        }
    }
    for(int i = 100; i < 1000; i++)
    {
        int val;
        retcode = mdpl::standardLibrary::BasicList::t_int::get(pList, static_cast<size_t>(i), &val);
        if(retcode)
        {
            printf("Failed to get data from basic list.\n");
            return;
        }
        if(val != i)
        {
            printf("basic list did not contain the right data.\n");
            return;
        }
    }

    //test deconstruction
    retcode = mdpl::standardLibrary::BasicList::t_int::destructor(pList);
    if(retcode)
    {
        printf("Failed to destroy basic list.\n");
        return;
    }

    if(mdpl::runtimeLib::allocator::doesAllocatorHaveActiveMemory())
    {
        printf("Did not deallocate all the memory. Potential memory leak.\n");
        return;
    }

    printf("Basic list passed all tests.\n");
}

int main()
{
    mdpl::runtimeLib::allocator::initialiseAllocator();

    testBasicList();

    mdpl::runtimeLib::allocator::destroyAllocator();

    return 0;
}