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

#include "../../vendor/utf8proc/utf8proc.h"

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
    const char* asciiCStr = "Hello world";
    const size_t asciiCStrNumCharacters = 11;
    const size_t asciiCStrNumBytes = 11;
    const char* emojiCStr = "😀😁😂😃";
    const size_t emojiCStrNumCharacters = 4;
    const size_t emojiCStrNumBytes = 16;
    //the two normalise strings should compare equal
    //the characters used are for the first string:  Å (swedish letter),         é (e acute)
    //                                second string: Å (angstrom prefix 10^-10), é (ascii e followed by acute modifier character)
    const char* normaliseCStr1 = "Åé";
    const char* normaliseCStr2 = "Åé";
    const size_t normaliseCStrNumCharacters1 = 2;
    const size_t normaliseCStrNumBytes1 = 4;
    const size_t normaliseCStrNumCharacters2 = 2;
    const size_t normaliseCStrNumBytes2 = 6;
    
    int retcode;
    
    //test 1: construct string with ascii data and test raw str
    mdpl::standardLibrary::String::StringRef asciiStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&asciiStr, asciiCStr, asciiCStrNumBytes, asciiCStrNumCharacters);
    if(retcode)
    {
        printf("Failed test 1. Error during constructing ascii str.\n");
    }
    if(asciiStr.s->rawStr->numBytes != asciiCStrNumBytes)
    {
        printf("Failed test 1. Ascii str's raw string had incorrect number of bytes.\n");
        return;
    }
    if(asciiStr.s->numCharacters != asciiCStrNumCharacters)
    {
        printf("Failed test 1. Ascii str's number of characters is incorrect.\n");
        return;
    }
    if(asciiStr.s->startByte != 0)
    {
        printf("Failed test 1. Ascii str's start byte is incorrect.\n");
        return;
    }
    if(asciiStr.s->endByte != asciiCStrNumBytes)
    {
        printf("Failed test 1. Ascii str's end byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < asciiCStrNumBytes; i++)
    {
        if(asciiStr.s->rawStr->str[i] != asciiCStr[i])
        {
            printf("Failed test 1. Ascii str did not contain the correct data.\n");
        }
    }

    //test 2: construct raw string with emoji data and test raw str
    mdpl::standardLibrary::String::StringRef emojiStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&emojiStr, emojiCStr, emojiCStrNumBytes, emojiCStrNumCharacters);
    if(retcode)
    {
        printf("Failed test 2. Error during constructing emijo string.\n");
        return;
    }
    if(emojiStr.s->rawStr->numBytes != emojiCStrNumBytes)
    {
        printf("Failed test 2. Emoji str's raw string had incorrect number of bytes.\n");
        return;
    }
    if(emojiStr.s->numCharacters != emojiCStrNumCharacters)
    {
        printf("Failed test 2. Emoji str's number of characters is incorrect.\n");
        return;
    }
    if(emojiStr.s->startByte != 0)
    {
        printf("Failed test 2. Emoji str's start byte is incorrect.\n");
        return;
    }
    if(emojiStr.s->endByte != emojiCStrNumBytes)
    {
        printf("Failed test 2. Emoji str's end byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < emojiCStrNumBytes; i++)
    {
        if(emojiStr.s->rawStr->str[i] != emojiCStr[i])
        {
            printf("Failed test 2. Emoji str did not contain the correct data.\n");
        }
    }

    //test 3: construct raw string with normalise data
    mdpl::standardLibrary::String::StringRef normaliseStr1 = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&normaliseStr1, normaliseCStr1, normaliseCStrNumBytes1, normaliseCStrNumCharacters1);
    if(retcode)
    {
        printf("Failed test 3. Error during constructing normalise str 1.\n");
        return;
    }
    if(normaliseStr1.s->rawStr->numBytes != normaliseCStrNumBytes1)
    {
        printf("Failed test 3. Normalise str 1's raw string had incorrect number of bytes.\n");
        return;
    }
    if(normaliseStr1.s->numCharacters != normaliseCStrNumCharacters1)
    {
        printf("Failed test 3. Normalise str 1's number of characters is incorrect.\n");
        return;
    }
    if(normaliseStr1.s->startByte != 0)
    {
        printf("Failed test 3. Normalise str 1's start byte is incorrect.\n");
        return;
    }
    if(normaliseStr1.s->endByte != normaliseCStrNumBytes1)
    {
        printf("Failed test 3. Normalise str 1's end byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < normaliseCStrNumBytes1; i++)
    {
        if(normaliseStr1.s->rawStr->str[i] != normaliseCStr1[i])
        {
            printf("Failed test 3. Normalise str 1 did not contain the correct data.\n");
        }
    }

    //test 4: construct raw string with normalise data
    mdpl::standardLibrary::String::StringRef normaliseStr2 = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&normaliseStr2, normaliseCStr2, normaliseCStrNumBytes2, normaliseCStrNumCharacters2);
    if(retcode)
    {
        printf("Failed test 4. Error during constructing normalise str 2.\n");
        return;
    }
    if(normaliseStr2.s->rawStr->numBytes != normaliseCStrNumBytes2)
    {
        printf("Failed test 4. Normalise str 2's raw string had incorrect number of bytes.\n");
        return;
    }
    if(normaliseStr2.s->numCharacters != normaliseCStrNumCharacters2)
    {
        printf("Failed test 4. Normalise str 2's number of characters is incorrect.\n");
        return;
    }
    if(normaliseStr2.s->startByte != 0)
    {
        printf("Failed test 4. Normalise str 2's start byte is incorrect.\n");
        return;
    }
    if(normaliseStr2.s->endByte != normaliseCStrNumBytes2)
    {
        printf("Failed test 4. Normalise str 2's end byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < normaliseCStrNumBytes2; i++)
    {
        if(normaliseStr2.s->rawStr->str[i] != normaliseCStr2[i])
        {
            printf("Failed test 4. Normalise str 2 did not contain the correct data.\n");
        }
    }

    //test 5: string normalisation
    retcode = mdpl::standardLibrary::String::internal::normaliseString(normaliseStr1.s);
    if(retcode)
    {
        printf("Failed test 5. Error during normalising normalise str 1.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::internal::normaliseString(normaliseStr2.s);
    if(retcode)
    {
        printf("Failed test 5. Error during normalising normalise str 2.\n");
        return;
    }
    if(normaliseStr1.s->normalisedStr->numBytes != normaliseStr2.s->normalisedStr->numBytes)
    {
        printf("Failed test 5. Normalised strings do not match. Different bumber of bytes.\n");
        return;
    }
    for(size_t i = 0; i < normaliseStr1.s->normalisedStr->numBytes; i++)
    {
        if(normaliseStr1.s->normalisedStr->str[i] != normaliseStr2.s->normalisedStr->str[i])
        {
            printf("Failed test 5. Normalised strings do not match.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::internal::normaliseString(asciiStr.s);
    if(retcode)
    {
        printf("Failed test 5. Error during normalising ascii string.\n");
        return;
    }
    if(asciiStr.s->normalisedStr->numBytes != asciiCStrNumBytes)
    {
        printf("Failed test 5. Normalised form of ascii string has the wrong number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < asciiStr.s->normalisedStr->numBytes; i++)
    {
        if(asciiStr.s->normalisedStr->str[i] != asciiCStr[i])
        {
            printf("Failed test 5. Normalised form of ascii string is incorrect.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::internal::normaliseString(emojiStr.s);
    if(retcode)
    {
        printf("Failed test 5. Error during normalising emoji string.\n");
        return;
    }
    if(emojiStr.s->normalisedStr->numBytes != emojiCStrNumBytes)
    {
        printf("Failed test 5. Normalised form of emoji string has the wrong number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < emojiStr.s->normalisedStr->numBytes; i++)
    {
        if(emojiStr.s->normalisedStr->str[i] != emojiCStr[i])
        {
            printf("Failed test 5. Normalised form of emoji string is incorrect.\n");
            return;
        }
    }

    //final test: deconstructions
    retcode = mdpl::standardLibrary::String::destroyStringRef(asciiStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(emojiStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(normaliseStr1);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(normaliseStr2);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
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