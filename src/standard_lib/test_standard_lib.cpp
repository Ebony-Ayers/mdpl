#include "../pch.hpp"
#include "../common/mdpl_common.hpp"

#define MDPL_GENERIC_TYPE_MACRO int
#define MDPL_GENERIC_TYPE_NAME_MACRO t_int
#include "basic_list_template.hpp"
#define MDPL_GENERIC_TYPE_MACRO int
#define MDPL_GENERIC_TYPE_NAME_MACRO t_int
#include "basic_list_template.cpp"

#include "string.hpp"

//this is not required in general use for use however is needed for debugging
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

    //test remove back
    for(int i = 999; i >= 0; i--)
    {
        int val;
        retcode = mdpl::standardLibrary::BasicList::t_int::back(pList, &val);
        if(retcode)
        {
            printf("Failed to get data from back basic list.\n");
            return;
        }
        if(val != i)
        {
            printf("basic list did not contain the right data.\n");
            return;
        }
        retcode = mdpl::standardLibrary::BasicList::t_int::removeBack(pList);
        if(retcode)
        {
            printf("Failed to remove last element of list.\n");
            return;
        }
    }
    if(!mdpl::standardLibrary::BasicList::t_int::isEmpty(pList))
    {
        printf("List should be empty but it isn't.\n");
        return;
    }

    //test prepend without reallocation
    for(int i = 0; i < 100; i++)
    {
        retcode = mdpl::standardLibrary::BasicList::t_int::prepend(&pList, i);
        if(retcode)
        {
            printf("Failed to prepend to basic list.\n");
            return;
        }
    }
    for(int i = 0; i < 100; i++)
    {
        int val;
        retcode = mdpl::standardLibrary::BasicList::t_int::get(pList, static_cast<size_t>(i), &val);
        if(retcode)
        {
            printf("Failed to get data from basic list.\n");
            return;
        }
        if(val != 99 - i)
        {
            printf("basic list did not contain the right data.\n");
            return;
        }
    }
    //test prepend with reallocation
    for(int i = 100; i < 2000; i++)
    {
        retcode = mdpl::standardLibrary::BasicList::t_int::prepend(&pList, i);
        if(retcode)
        {
            printf("Failed to prepend to basic list.\n");
            return;
        }
    }
    for(int i = 0; i < 2000; i++)
    {
        int val;
        retcode = mdpl::standardLibrary::BasicList::t_int::get(pList, static_cast<size_t>(i), &val);
        if(retcode)
        {
            printf("Failed to get data from basic list.\n");
            return;
        }
        if(val != 1999 - i)
        {
            printf("basic list did not contain the right data.\n");
            return;
        }
    }

    //test remove front
    for(int i = 1999; i >= 0; i--)
    {
        int val;
        retcode = mdpl::standardLibrary::BasicList::t_int::front(pList, &val);
        if(retcode)
        {
            printf("Failed to get data from front basic list.\n");
            return;
        }
        if(val != i)
        {
            printf("basic list did not contain the right data.\n");
            return;
        }
        retcode = mdpl::standardLibrary::BasicList::t_int::removeFront(pList);
        if(retcode)
        {
            printf("Failed to remove first element of list.\n");
            return;
        }
    }
    if(!mdpl::standardLibrary::BasicList::t_int::isEmpty(pList))
    {
        printf("List should be empty but it isn't.\n");
        return;
    }

    //test setting data
    for(int i = 0; i < 10; i++)
    {
        retcode = mdpl::standardLibrary::BasicList::t_int::append(&pList, i);
        if(retcode)
        {
            printf("Failed to append to basic list.\n");
            return;
        }
    }
    for(int i = 0; i < 10; i++)
    {
        retcode = mdpl::standardLibrary::BasicList::t_int::set(pList, static_cast<size_t>(i), i + 10);
        if(retcode)
        {
            printf("Failed to write to basic list.\n");
            return;
        }
    }
    for(int i = 0; i < 10; i++)
    {
        int val;
        retcode = mdpl::standardLibrary::BasicList::t_int::get(pList, static_cast<size_t>(i), &val);
        if(retcode)
        {
            printf("Failed to get data from basic list.\n");
            return;
        }
        if(val != i + 10)
        {
            printf("basic list did not contain the right data.\n");
            return;
        }
    }

    //test clear
    retcode = mdpl::standardLibrary::BasicList::t_int::clear(pList);
    if(retcode)
    {
        printf("Failed to clear list.\n");
        return;
    }
    else if(!mdpl::standardLibrary::BasicList::t_int::isEmpty(pList))
    {
        printf("List is not empty when it should be.\n");
        return;
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

void testString()
{
    const char* cstr = "Hello world";
    int retcode;

    mdpl::standardLibrary::String::RawString* rawStr;
    retcode = mdpl::standardLibrary::String::createRawString(&rawStr, cstr, 11);
    if(retcode)
    {
        printf("Error 1\n");
        return;
    }

    if(rawStr->numBytes != 11)
    {
        printf("Error 2\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        printf("%c", rawStr->str[i]);
    }
    printf("\n");

    mdpl::standardLibrary::String::String str = {};
    retcode = mdpl::standardLibrary::String::createString(&str, 11, 0, 11, rawStr);
    if(retcode)
    {
        printf("Error 3\n");
        return;
    }
    if(str.rawStr != rawStr)
    {
        printf("Error 4\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        printf("%c", str.rawStr->str[i]);
    }
    printf("\n");

    mdpl::standardLibrary::String::String copyOfStr = {};
    retcode = mdpl::standardLibrary::String::copyString(&str, &copyOfStr);
    if(retcode)
    {
        printf("Error 6\n");
        return;
    }
    if(copyOfStr.rawStr != rawStr)
    {
        printf("Error 7\n");
        return;
    }

    retcode = mdpl::standardLibrary::String::destroyString(&str);
    if(retcode)
    {
        printf("Error 8\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&copyOfStr);
    if(retcode)
    {
        printf("Error 9\n");
        return;
    }

    if(mdpl::runtimeLib::allocator::doesAllocatorHaveActiveMemory())
    {
        printf("Did not deallocate all the memory. Potential memory leak.\n");
        return;
    }

    printf("String passed all tests.\n");
}

int main(int /*argc*/, char** /*argv*/)
{
    mdpl::runtimeLib::allocator::initialiseAllocator();

    testBasicList();

    testString();

    mdpl::runtimeLib::allocator::destroyAllocator();

    return 0;
}