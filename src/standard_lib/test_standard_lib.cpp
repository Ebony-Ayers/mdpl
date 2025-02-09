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

    //test 1: construct raw string with ascii data
    mdpl::standardLibrary::String::RawString* asciiRawStr;
    retcode = mdpl::standardLibrary::String::createRawString(&asciiRawStr, asciiCStr, 11);
    if(retcode)
    {
        printf("Failed test 1. Error during constructing raw string.\n");
        return;
    }
    if(asciiRawStr->numBytes != 11)
    {
        printf("Failed test 1. Raw string had incorrect number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(asciiRawStr->str[i] != asciiCStr[i])
        {
            printf("Failed test 1. Raw string did not contain the correct data.");
            return;
        }
    }

    //test 2: construct string with ascii data
    mdpl::standardLibrary::String::String asciiStr = {};
    retcode = mdpl::standardLibrary::String::createString(&asciiStr, asciiCStrNumCharacters, 0, asciiCStrNumBytes, asciiRawStr);
    if(retcode)
    {
        printf("Failed test 2. Error during constructing string.\n");
        return;
    }
    if(asciiStr.rawStr != asciiRawStr)
    {
        printf("Failed test 2. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(asciiStr.numCharacters != asciiCStrNumCharacters)
    {
        printf("Failed test 2. Number of characters is incorrect.\n");
        return;
    }
    if(asciiStr.startByte != 0)
    {
        printf("Failed test 2. Start byte is incorrect.\n");
        return;
    }
    if(asciiStr.endByte != asciiCStrNumBytes)
    {
        printf("Failed test 2. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(asciiStr.rawStr->str[i] != asciiCStr[i])
        {
            printf("Failed test 2. String did not contain the correct data.\n");
        }
    }

    //test 3: copy a string with ascii data
    mdpl::standardLibrary::String::String copyOfAsciiStr = {};
    retcode = mdpl::standardLibrary::String::copyString(&asciiStr, &copyOfAsciiStr);
    if(retcode)
    {
        printf("Failed test 3. Error during constructing string.\n");
        return;
    }
    if(copyOfAsciiStr.rawStr != asciiRawStr)
    {
        printf("Failed test 3. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(copyOfAsciiStr.numCharacters != asciiCStrNumCharacters)
    {
        printf("Failed test 3. Number of characters is incorrect.\n");
        return;
    }
    if(copyOfAsciiStr.startByte != 0)
    {
        printf("Failed test 3. Start byte is incorrect.\n");
        return;
    }
    if(copyOfAsciiStr.endByte != asciiCStrNumBytes)
    {
        printf("Failed test 3. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(copyOfAsciiStr.rawStr->str[i] != asciiCStr[i])
        {
            printf("Failed test 3. String did not contain the correct data.\n");
        }
    }


    //test 4: construct raw string with emoji data
    mdpl::standardLibrary::String::RawString* emojiRawStr;
    retcode = mdpl::standardLibrary::String::createRawString(&emojiRawStr, emojiCStr, 11);
    if(retcode)
    {
        printf("Failed test 4. Error during constructing raw string.\n");
        return;
    }
    if(emojiRawStr->numBytes != 11)
    {
        printf("Failed test 4. Raw string had incorrect number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(emojiRawStr->str[i] != emojiCStr[i])
        {
            printf("Failed test 4. Raw string did not contain the correct data.");
            return;
        }
    }

    //test 5: construct string with emoji data
    mdpl::standardLibrary::String::String emojiStr = {};
    retcode = mdpl::standardLibrary::String::createString(&emojiStr, emojiCStrNumCharacters, 0, emojiCStrNumBytes, emojiRawStr);
    if(retcode)
    {
        printf("Failed test 5. Error during constructing string.\n");
        return;
    }
    if(emojiStr.rawStr != emojiRawStr)
    {
        printf("Failed test 5. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(emojiStr.numCharacters != emojiCStrNumCharacters)
    {
        printf("Failed test 5. Number of characters is incorrect.\n");
        return;
    }
    if(emojiStr.startByte != 0)
    {
        printf("Failed test 5. Start byte is incorrect.\n");
        return;
    }
    if(emojiStr.endByte != emojiCStrNumBytes)
    {
        printf("Failed test 5. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(emojiStr.rawStr->str[i] != emojiCStr[i])
        {
            printf("Failed test 5. String did not contain the correct data.\n");
        }
    }

    //test 6: copy a string with emoji data
    mdpl::standardLibrary::String::String copyOfEmojiStr = {};
    retcode = mdpl::standardLibrary::String::copyString(&emojiStr, &copyOfEmojiStr);
    if(retcode)
    {
        printf("Failed test 6. Error during constructing string.\n");
        return;
    }
    if(copyOfEmojiStr.rawStr != emojiRawStr)
    {
        printf("Failed test 6. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(copyOfEmojiStr.numCharacters != emojiCStrNumCharacters)
    {
        printf("Failed test 6. Number of characters is incorrect.\n");
        return;
    }
    if(copyOfEmojiStr.startByte != 0)
    {
        printf("Failed test 6. Start byte is incorrect.\n");
        return;
    }
    if(copyOfEmojiStr.endByte != emojiCStrNumBytes)
    {
        printf("Failed test 6. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(copyOfEmojiStr.rawStr->str[i] != emojiCStr[i])
        {
            printf("Failed test 6. String did not contain the correct data.\n");
        }
    }

    //test 7: construct raw string with normalise data
    mdpl::standardLibrary::String::RawString* normaliseRawStr1;
    retcode = mdpl::standardLibrary::String::createRawString(&normaliseRawStr1, normaliseCStr1, 11);
    if(retcode)
    {
        printf("Failed test 7. Error during constructing raw string.\n");
        return;
    }
    if(normaliseRawStr1->numBytes != 11)
    {
        printf("Failed test 7. Raw string had incorrect number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(normaliseRawStr1->str[i] != normaliseCStr1[i])
        {
            printf("Failed test 7. Raw string did not contain the correct data.");
            return;
        }
    }

    //test 8: construct string with normalise data
    mdpl::standardLibrary::String::String normaliseStr1 = {};
    retcode = mdpl::standardLibrary::String::createString(&normaliseStr1, normaliseCStrNumCharacters1, 0, normaliseCStrNumBytes1, normaliseRawStr1);
    if(retcode)
    {
        printf("Failed test 8. Error during constructing string.\n");
        return;
    }
    if(normaliseStr1.rawStr != normaliseRawStr1)
    {
        printf("Failed test 8. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(normaliseStr1.numCharacters != normaliseCStrNumCharacters1)
    {
        printf("Failed test 8. Number of characters is incorrect.\n");
        return;
    }
    if(normaliseStr1.startByte != 0)
    {
        printf("Failed test 8. Start byte is incorrect.\n");
        return;
    }
    if(normaliseStr1.endByte != normaliseCStrNumBytes1)
    {
        printf("Failed test 8. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(normaliseStr1.rawStr->str[i] != normaliseCStr1[i])
        {
            printf("Failed test 8. String did not contain the correct data.\n");
        }
    }

    //test 9: copy a string with normalise data
    mdpl::standardLibrary::String::String copyOfNormaliseStr1 = {};
    retcode = mdpl::standardLibrary::String::copyString(&normaliseStr1, &copyOfNormaliseStr1);
    if(retcode)
    {
        printf("Failed test 9. Error during constructing string.\n");
        return;
    }
    if(copyOfNormaliseStr1.rawStr != normaliseRawStr1)
    {
        printf("Failed test 9. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(copyOfNormaliseStr1.numCharacters != normaliseCStrNumCharacters1)
    {
        printf("Failed test 9. Number of characters is incorrect.\n");
        return;
    }
    if(copyOfNormaliseStr1.startByte != 0)
    {
        printf("Failed test 9. Start byte is incorrect.\n");
        return;
    }
    if(copyOfNormaliseStr1.endByte != normaliseCStrNumBytes1)
    {
        printf("Failed test 9. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(copyOfNormaliseStr1.rawStr->str[i] != normaliseCStr1[i])
        {
            printf("Failed test 9. String did not contain the correct data.\n");
        }
    }


    //test 12: construct raw string with normalise data
    mdpl::standardLibrary::String::RawString* normaliseRawStr2;
    retcode = mdpl::standardLibrary::String::createRawString(&normaliseRawStr2, normaliseCStr2, 11);
    if(retcode)
    {
        printf("Failed test 12. Error during constructing raw string.\n");
        return;
    }
    if(normaliseRawStr2->numBytes != 11)
    {
        printf("Failed test 12. Raw string had incorrect number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(normaliseRawStr2->str[i] != normaliseCStr2[i])
        {
            printf("Failed test 12. Raw string did not contain the correct data.");
            return;
        }
    }

    //test 11: construct string with normalise data
    mdpl::standardLibrary::String::String normaliseStr2 = {};
    retcode = mdpl::standardLibrary::String::createString(&normaliseStr2, normaliseCStrNumCharacters2, 0, normaliseCStrNumBytes2, normaliseRawStr2);
    if(retcode)
    {
        printf("Failed test 11. Error during constructing string.\n");
        return;
    }
    if(normaliseStr2.rawStr != normaliseRawStr2)
    {
        printf("Failed test 11. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(normaliseStr2.numCharacters != normaliseCStrNumCharacters2)
    {
        printf("Failed test 11. Number of characters is incorrect.\n");
        return;
    }
    if(normaliseStr2.startByte != 0)
    {
        printf("Failed test 11. Start byte is incorrect.\n");
        return;
    }
    if(normaliseStr2.endByte != normaliseCStrNumBytes2)
    {
        printf("Failed test 11. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(normaliseStr2.rawStr->str[i] != normaliseCStr2[i])
        {
            printf("Failed test 11. String did not contain the correct data.\n");
        }
    }

    //test 12: copy a string with normalise data
    mdpl::standardLibrary::String::String copyOfNormaliseStr2 = {};
    retcode = mdpl::standardLibrary::String::copyString(&normaliseStr2, &copyOfNormaliseStr2);
    if(retcode)
    {
        printf("Failed test 12. Error during constructing string.\n");
        return;
    }
    if(copyOfNormaliseStr2.rawStr != normaliseRawStr2)
    {
        printf("Failed test 12. Raw string pointer did not point to correct location.\n");
        return;
    }
    if(copyOfNormaliseStr2.numCharacters != normaliseCStrNumCharacters2)
    {
        printf("Failed test 12. Number of characters is incorrect.\n");
        return;
    }
    if(copyOfNormaliseStr2.startByte != 0)
    {
        printf("Failed test 12. Start byte is incorrect.\n");
        return;
    }
    if(copyOfNormaliseStr2.endByte != normaliseCStrNumBytes2)
    {
        printf("Failed test 12. End byte is incorrect.\n");
        return;
    }
    for(size_t i = 0; i < 11; i++)
    {
        if(copyOfNormaliseStr2.rawStr->str[i] != normaliseCStr2[i])
        {
            printf("Failed test 12. String did not contain the correct data.\n");
        }
    }

    //test 13: string normalisation
    retcode = mdpl::standardLibrary::String::normaliseString(&normaliseStr1);
    if(retcode)
    {
        printf("Failed test 13. Error during normalising string 1.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::normaliseString(&normaliseStr2);
    if(retcode)
    {
        printf("Failed test 13. Error during normalising string 2.\n");
        return;
    }
    if(normaliseStr1.normalisedStr->numBytes != normaliseStr2.normalisedStr->numBytes)
    {
        printf("Failed test 13. Normalised strings do not match. Different bumber of bytes.\n");
        return;
    }
    if(normaliseStr1.normalisedStr->flags & mdpl::standardLibrary::String::RawStringFlags::isNormalised == 0)
    {
        printf("Failed test 13. Normalised string 1 is not flagged as normalised.\n");
        return;
    }
    if(normaliseStr2.normalisedStr->flags & mdpl::standardLibrary::String::RawStringFlags::isNormalised == 0)
    {
        printf("Failed test 13. Normalised string 2 is not flagged as normalised.\n");
        return;
    }
    for(size_t i = 0; i < normaliseStr1.normalisedStr->numBytes; i++)
    {
        if(normaliseStr1.normalisedStr->str[i] != normaliseStr2.normalisedStr->str[i])
        {
            printf("Failed test 13. Normalised strings do not match.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::normaliseString(&asciiStr);
    if(retcode)
    {
        printf("Failed test 13. Error during normalising ascii string.\n");
        return;
    }
    if(asciiStr.normalisedStr->flags & mdpl::standardLibrary::String::RawStringFlags::isNormalised == 0)
    {
        printf("Failed test 13. Ascii string is not flagged as normalised.\n");
        return;
    }
    if(asciiStr.normalisedStr->numBytes != asciiCStrNumBytes)
    {
        printf("Failed test 13. Normalised form of ascii string has the wrong number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < asciiStr.normalisedStr->numBytes; i++)
    {
        if(asciiStr.normalisedStr->str[i] != asciiCStr[i])
        {
            printf("Failed test 13. Normalised form of ascii string is incorrect.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::normaliseString(&emojiStr);
    if(retcode)
    {
        printf("Failed test 13. Error during normalising emoji string.\n");
        return;
    }
    if(emojiStr.normalisedStr->flags & mdpl::standardLibrary::String::RawStringFlags::isNormalised == 0)
    {
        printf("Failed test 13. Emoji string is not flagged as normalised.\n");
        return;
    }
    if(emojiStr.normalisedStr->numBytes != emojiCStrNumBytes)
    {
        printf("Failed test 13. Normalised form of emoji string has the wrong number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < emojiStr.normalisedStr->numBytes; i++)
    {
        if(emojiStr.normalisedStr->str[i] != emojiCStr[i])
        {
            printf("Failed test 13. Normalised form of emoji string is incorrect.\n");
            return;
        }
    }

    //final test: deconstructions
    retcode = mdpl::standardLibrary::String::destroyString(&asciiStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&copyOfAsciiStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of coppied string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&emojiStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&copyOfEmojiStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of coppied string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&normaliseStr1);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&copyOfNormaliseStr1);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of coppied string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&normaliseStr2);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyString(&copyOfNormaliseStr2);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of coppied string containing ascii data.\n");
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