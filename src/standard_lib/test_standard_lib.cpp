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
    bool result;
    
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

    //test 6: isAscii
    retcode = mdpl::standardLibrary::String::isAscii(asciiStr, &result);
    if(retcode)
    {
        printf("Failed test 6. Error during isAscii call on ascii str.\n");
        return;
    }
    if(!result)
    {
        printf("Failed test 6. isAscii result is wrong for ascii str.\n");
        return;
    }
    if(! ((asciiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) || (asciiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii)) )
    {
        printf("Failed test 6. Flags incorrectly set for ascii str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isAscii(emojiStr, &result);
    if(retcode)
    {
        printf("Failed test 6. Error during isAscii call on emoji str.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 6. isAscii result is wrong for emoji str.\n");
        return;
    }
    if(! ((emojiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) || (emojiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii)) )
    {
        printf("Failed test 6. Flags incorrectly set for emoji str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isAscii(normaliseStr1, &result);
    if(retcode)
    {
        printf("Failed test 6. Error during isAscii call on normalise str 1.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 6. isAscii result is wrong for normalise str 1.\n");
        return;
    }
    if(! ((normaliseStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) || (normaliseStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii)) )
    {
        printf("Failed test 6. Flags incorrectly set for normalise str 1.\n");
        return;
    }

    //setup for steps 7-9
    mdpl::standardLibrary::String::StringRef validDecimalStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&validDecimalStr, "123456789", 9, 9);
    if(retcode) { printf("Failed test 7. Error during constructing valid decimal str.\n"); return; }
    mdpl::standardLibrary::String::StringRef validIntStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&validIntStr, "-123456789", 10, 10);
    if(retcode) { printf("Failed test 7. Error during constructing valid int str.\n"); return; }
    mdpl::standardLibrary::String::StringRef invalidIntStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&invalidIntStr, "-1234567-89", 11, 11);
    if(retcode) { printf("Failed test 7. Error during constructing invalid int str.\n"); return; }
    mdpl::standardLibrary::String::StringRef validFloatStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&validFloatStr, "-1234.56789", 11, 11);
    if(retcode) { printf("Failed test 7. Error during constructing valid float str.\n"); return; }
    mdpl::standardLibrary::String::StringRef invalidFloatStr1 = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&invalidFloatStr1, "-1234.567.89", 12, 12);
    if(retcode) { printf("Failed test 7. Error during constructing invalid float str 1.\n"); return; }
    mdpl::standardLibrary::String::StringRef invalidFloatStr2 = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&invalidFloatStr2, "-1234.567-89", 12, 12);
    if(retcode) { printf("Failed test 7. Error during constructing invalid float str 2.\n"); return; }
    
    //test 7: isValidDecimal
    retcode = mdpl::standardLibrary::String::isValidDecimal(validDecimalStr, &result);
    if(retcode)
    {
        printf("Failed test 7. Error during isValidDecimal call on valid decimal str.\n");
        return;
    }
    if(!result)
    {
        printf("Failed test 7. isValidDecimal result is wrong for valid decimal str.\n");
        return;
    }
    if(! ((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) || (validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal)) )
    {
        printf("Failed test 7. Flags incorrectly set for valid decimal str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidDecimal(validIntStr, &result);
    if(retcode)
    {
        printf("Failed test 7. Error during isValidDecimal call on valid int str.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 7. isValidDecimal result is wrong for valid int str.\n");
        return;
    }
    if(! ((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) || (validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal)) )
    {
        printf("Failed test 7. Flags incorrectly set for valid int str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidDecimal(invalidIntStr, &result);
    if(retcode)
    {
        printf("Failed test 7. Error during isValidDecimal call on invalid int str.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 7. isValidDecimal result is wrong for invalid int str.\n");
        return;
    }
    if(! ((invalidIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) || (invalidIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal)) )
    {
        printf("Failed test 7. Flags incorrectly set for invalid int str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidDecimal(validFloatStr, &result);
    if(retcode)
    {
        printf("Failed test 7. Error during isValidDecimal call on valid float str.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 7. isValidDecimal result is wrong for valid float str.\n");
        return;
    }
    if(! ((validFloatStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) || (validFloatStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal)) )
    {
        printf("Failed test 7. Flags incorrectly set for valid float str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidDecimal(invalidFloatStr1, &result);
    if(retcode)
    {
        printf("Failed test 7. Error during isValidDecimal call on invalid float str 1.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 7. isValidDecimal result is wrong for invalid float str 1.\n");
        return;
    }
    if(! ((invalidFloatStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) || (invalidFloatStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal)) )
    {
        printf("Failed test 7. Flags incorrectly set for invalid float str 1.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidDecimal(invalidFloatStr2, &result);
    if(retcode)
    {
        printf("Failed test 7. Error during isValidDecimal call on invalid float str 2.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 7. isValidDecimal result is wrong for invalid float str 2.\n");
        return;
    }
    if(! ((invalidFloatStr2.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) || (invalidFloatStr2.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal)) )
    {
        printf("Failed test 7. Flags incorrectly set for invalid float str 2.\n");
        return;
    }

    //test 8: isValidInt
    retcode = mdpl::standardLibrary::String::isValidInt(validDecimalStr, &result);
    if(retcode)
    {
        printf("Failed test 8. Error during isValidInt call on valid decimal str.\n");
        return;
    }
    if(!result)
    {
        printf("Failed test 8. isValidInt result is wrong for valid decimal str.\n");
        return;
    }
    if(! ((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) || (validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt)) )
    {
        printf("Failed test 8. Flags incorrectly set for valid decimal str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidInt(validIntStr, &result);
    if(retcode)
    {
        printf("Failed test 8. Error during isValidInt call on valid int str.\n");
        return;
    }
    if(!result)
    {
        printf("Failed test 8. isValidInt result is wrong for valid int str.\n");
        return;
    }
    if(! ((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) || (validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt)) )
    {
        printf("Failed test 8. Flags incorrectly set for valid int str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidInt(invalidIntStr, &result);
    if(retcode)
    {
        printf("Failed test 8. Error during isValidInt call on invalid int str.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 8. isValidInt result is wrong for invalid int str.\n");
        return;
    }
    if(! ((invalidIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) || (invalidIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt)) )
    {
        printf("Failed test 8. Flags incorrectly set for invalid int str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidInt(validFloatStr, &result);
    if(retcode)
    {
        printf("Failed test 8. Error during isValidInt call on valid float str.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 8. isValidInt result is wrong for valid float str.\n");
        return;
    }
    if(! ((validFloatStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) || (validFloatStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt)) )
    {
        printf("Failed test 8. Flags incorrectly set for valid float str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidInt(invalidFloatStr1, &result);
    if(retcode)
    {
        printf("Failed test 8. Error during isValidInt call on invalid float str 1.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 8. isValidInt result is wrong for invalid float str 1.\n");
        return;
    }
    if(! ((invalidFloatStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) || (invalidFloatStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt)) )
    {
        printf("Failed test 8. Flags incorrectly set for invalid float str 1.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidInt(invalidFloatStr2, &result);
    if(retcode)
    {
        printf("Failed test 8. Error during isValidInt call on invalid float str 2.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 8. isValidInt result is wrong for invalid float str 2.\n");
        return;
    }
    if(! ((invalidFloatStr2.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) || (invalidFloatStr2.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt)) )
    {
        printf("Failed test 8. Flags incorrectly set for invalid float str 2.\n");
        return;
    }

    //test 9: isValidFloat
    retcode = mdpl::standardLibrary::String::isValidFloat(validDecimalStr, &result);
    if(retcode)
    {
        printf("Failed test 9. Error during isValidFloat call on valid decimal str.\n");
        return;
    }
    if(!result)
    {
        printf("Failed test 9. isValidFloat result is wrong for valid decimal str.\n");
        return;
    }
    if(! ((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) || (validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat)) )
    {
        printf("Failed test 9. Flags incorrectly set for valid decimal str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidFloat(validIntStr, &result);
    if(retcode)
    {
        printf("Failed test 9. Error during isValidFloat call on valid int str.\n");
        return;
    }
    if(!result)
    {
        printf("Failed test 9. isValidFloat result is wrong for valid int str.\n");
        return;
    }
    if(! ((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) || (validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat)) )
    {
        printf("Failed test 9. Flags incorrectly set for valid int str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidFloat(invalidIntStr, &result);
    if(retcode)
    {
        printf("Failed test 9. Error during isValidFloat call on invalid int str.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 9. isValidFloat result is wrong for invalid int str.\n");
        return;
    }
    if(! ((invalidIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) || (invalidIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat)) )
    {
        printf("Failed test 9. Flags incorrectly set for invalid int str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidFloat(validFloatStr, &result);
    if(retcode)
    {
        printf("Failed test 9. Error during isValidFloat call on valid float str.\n");
        return;
    }
    if(!result)
    {
        printf("Failed test 9. isValidFloat result is wrong for valid float str.\n");
        return;
    }
    if(! ((validFloatStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) || (validFloatStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat)) )
    {
        printf("Failed test 9. Flags incorrectly set for valid float str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidFloat(invalidFloatStr1, &result);
    if(retcode)
    {
        printf("Failed test 9. Error during isValidFloat call on invalid float str 1.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 9. isValidFloat result is wrong for invalid float str 1.\n");
        return;
    }
    if(! ((invalidFloatStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) || (invalidFloatStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat)) )
    {
        printf("Failed test 9. Flags incorrectly set for invalid float str 1.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isValidFloat(invalidFloatStr2, &result);
    if(retcode)
    {
        printf("Failed test 9. Error during isValidFloat call on invalid float str 2.\n");
        return;
    }
    if(result)
    {
        printf("Failed test 9. isValidFloat result is wrong for invalid float str 2.\n");
        return;
    }
    if(! ((invalidFloatStr2.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) || (invalidFloatStr2.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat)) )
    {
        printf("Failed test 9. Flags incorrectly set for invalid float str 2.\n");
        return;
    }

    //setup for steps 10-11
    mdpl::standardLibrary::String::StringRef lowerCaseStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&lowerCaseStr, "street straße δρόμος", 27, 20);
    if(retcode) { printf("Failed test 10. Error during constructing lower cases str.\n"); return; }
    mdpl::standardLibrary::String::StringRef upperCaseStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&upperCaseStr, "STREET STRASSE ΔΡΌΜΟΣ", 27, 21);
    if(retcode) { printf("Failed test 10. Error during constructing upper cases str.\n"); return; }
    mdpl::standardLibrary::String::StringRef mixedCaseStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&mixedCaseStr, "Street Straße Δρόμος", 27, 20);
    if(retcode) { printf("Failed test 10. Error during constructing mixed cases str.\n"); return; }

    //test 10: isLower
    retcode = mdpl::standardLibrary::String::isLower(lowerCaseStr, &result);
    if(!result)
    {
        printf("Failed test 10. isLower is incorrect for lower case str.\n");
        return;
    }
    if((lowerCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isLower) && !(lowerCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isLower))
    {
        printf("Failed test 10. isLower incorrectly set flags for lower case str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isLower(upperCaseStr, &result);
    if(result)
    {
        printf("Failed test 10. isLower is incorrect for upper case str.\n");
        return;
    }
    if((upperCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isLower) && (upperCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isLower))
    {
        printf("Failed test 10. isLower incorrectly set flags for upper case str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isLower(mixedCaseStr, &result);
    if(result)
    {
        printf("Failed test 10. isLower is incorrect for mixed case str.\n");
        return;
    }
    if((mixedCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isLower) && (mixedCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isLower))
    {
        printf("Failed test 10. isLower incorrectly set flags for mixed case str.\n");
        return;
    }

    //test 11: isUpper
    retcode = mdpl::standardLibrary::String::isUpper(lowerCaseStr, &result);
    if(result)
    {
        printf("Failed test 11. isUpper is incorrect for lower case str.\n");
        return;
    }
    if((lowerCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isUpper) && (lowerCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isUpper))
    {
        printf("Failed test 10. isUpper incorrectly set flags for lower case str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isUpper(upperCaseStr, &result);
    if(!result)
    {
        printf("Failed test 11. isUpper is incorrect for upper case str.\n");
        return;
    }
    if((upperCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isUpper) && !(upperCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isUpper))
    {
        printf("Failed test 10. isUpper incorrectly set flags for upper case str.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isUpper(mixedCaseStr, &result);
    if(result)
    {
        printf("Failed test 11. isUpper is incorrect for mixed case str.\n");
        return;
    }
    if((mixedCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isUpper) && (mixedCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isUpper))
    {
        printf("Failed test 10. isUpper incorrectly set flags for mixed case str.\n");
        return;
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

    retcode = mdpl::standardLibrary::String::destroyStringRef(validDecimalStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(validIntStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(invalidIntStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(validFloatStr);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(invalidFloatStr1);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(invalidFloatStr2);
    if(retcode)
    {
        printf("Failed final test. Error during destuction of string containing ascii data.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(lowerCaseStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of lower case string.\n");
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(upperCaseStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of upper case string.\n");
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(mixedCaseStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of mixed case string.\n");
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