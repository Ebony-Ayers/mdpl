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

#define MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(sr) sr.s->flagsSet &= ~mdpl::standardLibrary::String::StringFlags::isAscii; sr.s->flagsData &= ~mdpl::standardLibrary::String::StringFlags::isAscii;

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

    //setup for steps 13-15
    mdpl::standardLibrary::String::StringRef validDecimalStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&validDecimalStr, "123456789", 9, 9);
    if(retcode) { printf("Failed test 13. Error during constructing valid decimal str.\n"); return; }
    mdpl::standardLibrary::String::StringRef validIntStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&validIntStr, "-123456789", 10, 10);
    if(retcode) { printf("Failed test 13. Error during constructing valid int str.\n"); return; }
    mdpl::standardLibrary::String::StringRef invalidIntStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&invalidIntStr, "-1234567-89", 11, 11);
    if(retcode) { printf("Failed test 13. Error during constructing invalid int str.\n"); return; }
    mdpl::standardLibrary::String::StringRef validFloatStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&validFloatStr, "-1234.56789", 11, 11);
    if(retcode) { printf("Failed test 13. Error during constructing valid float str.\n"); return; }
    mdpl::standardLibrary::String::StringRef invalidFloatStr1 = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&invalidFloatStr1, "-1234.567.89", 12, 12);
    if(retcode) { printf("Failed test 13. Error during constructing invalid float str 1.\n"); return; }
    mdpl::standardLibrary::String::StringRef invalidFloatStr2 = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&invalidFloatStr2, "-1234.567-89", 12, 12);
    if(retcode) { printf("Failed test 13. Error during constructing invalid float str 2.\n"); return; }
    //setup for steps 16-17
    mdpl::standardLibrary::String::StringRef lowerCaseStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&lowerCaseStr, "street straße δρόμος", 27, 20);
    if(retcode) { printf("Failed test 16. Error during constructing lower cases str.\n"); return; }
    mdpl::standardLibrary::String::StringRef upperCaseStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&upperCaseStr, "STREET STRASSE ΔΡΌΜΟΣ", 27, 21);
    if(retcode) { printf("Failed test 16. Error during constructing upper cases str.\n"); return; }
    mdpl::standardLibrary::String::StringRef mixedCaseStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&mixedCaseStr, "Street Straße Δρόμος", 27, 20);
    if(retcode) { printf("Failed test 16. Error during constructing mixed cases str.\n"); return; }
    //setup for test 20-21
    mdpl::standardLibrary::String::StringRef alphaStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&alphaStr, "helloWorld", 10, 10);
    if(retcode) { printf("Failed test 20. Error during constructing alpha str.\n"); return; }
    mdpl::standardLibrary::String::StringRef alphaNumStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&alphaNumStr, "helloWorld1234", 14, 14);
    if(retcode) { printf("Failed test 20. Error during constructing alpha numeric str.\n"); return; }
    //setup for steps 22-23
    mdpl::standardLibrary::String::StringRef lowerCasePrefixStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&lowerCasePrefixStr, "street", 6, 6);
    if(retcode) { printf("Failed test 16. Error during constructing lower cases str.\n"); return; }
    mdpl::standardLibrary::String::StringRef upperCasePrefixStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&upperCasePrefixStr, "STREET", 6, 6);
    if(retcode) { printf("Failed test 16. Error during constructing lower cases str.\n"); return; }
    mdpl::standardLibrary::String::StringRef lowerCaseSuffixStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&lowerCaseSuffixStr, "δρόμος", 12, 6);
    if(retcode) { printf("Failed test 16. Error during constructing upper cases str.\n"); return; }
    mdpl::standardLibrary::String::StringRef upperCaseSuffixStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&upperCaseSuffixStr, "ΔΡΌΜΟΣ", 12, 6);
    if(retcode) { printf("Failed test 16. Error during constructing upper cases str.\n"); return; }

    //setup for test 5 - 8
    mdpl::standardLibrary::String::StringIterator asciiStrForwardIt = {asciiStr.s, 6, 6, 1};
    mdpl::standardLibrary::String::StringIterator asciiStrReverseIt = {asciiStr.s, 10, 10, -1};
    mdpl::standardLibrary::String::StringIterator asciiStrDoubleIt = {asciiStr.s, 0, 0, 2};
    mdpl::standardLibrary::String::StringIterator normaliseStr1It = {normaliseStr1.s, 0, 0, 1};
    mdpl::standardLibrary::String::StringIterator nonAsciiStrIt = {lowerCaseStr.s, 7, 7, 1};
    mdpl::standardLibrary::String::Character c;

    //test 5: getCurrent
    retcode = mdpl::standardLibrary::String::getCurrent(&asciiStrForwardIt, &c);
    if(retcode)
    {
        printf("failed test 5: Error during getCurrent on asciiStrForwardIt.\n");
        return;
    }
    if(c.codepoint != 'w')
    {
        printf("failed test 5: Character did not contain the correct information for asciiStrForwardIt.\n");
        return;
    }

    retcode = mdpl::standardLibrary::String::getCurrent(&asciiStrReverseIt, &c);
    if(retcode)
    {
        printf("failed test 5: Error during getCurrent on asciiStrReverseIt.\n");
        return;
    }
    if(c.codepoint != 'd')
    {
        printf("failed test 5: Character did not contain the correct information for asciiStrReverseIt.\n");
        return;
    }

    retcode = mdpl::standardLibrary::String::getCurrent(&asciiStrDoubleIt, &c);
    if(retcode)
    {
        printf("failed test 5: Error during getCurrent on asciiStrDoubleIt.\n");
        return;
    }
    if(c.codepoint != 'H')
    {
        printf("failed test 5: Character did not contain the correct information for asciiStrDoubleIt.\n");
        return;
    }

    retcode = mdpl::standardLibrary::String::getCurrent(&normaliseStr1It, &c);
    if(retcode)
    {
        printf("failed test 5: Error during getCurrent on normaliseStr1It.\n");
        return;
    }
    if(c.codepoint != 197)
    {
        printf("failed test 5: Character did not contain the correct information for normaliseStr1It.\n");
        return;
    }

    //test 6: next
    retcode = mdpl::standardLibrary::String::next(&asciiStrForwardIt);
    if(retcode)
    {
        printf("failed test 6: Error during next on asciiStrForwardIt.\n");
        return;
    }
    if(asciiStrForwardIt.str != asciiStr.s)
    {
        printf("failed test 6: String reference changed in asciiStrForwardIt.\n");
        return;
    }
    if(asciiStrForwardIt.byteIndex != 7)
    {
        printf("failed test 6: New byte index is incorrect for asciiStrForwardIt.\n");
        return;
    }
    if(asciiStrForwardIt.characterIndex != 7)
    {
        printf("failed test 6: New character index is incorrect for asciiStrForwardIt.\n");
        return;
    }
    if(asciiStrForwardIt.step != 1)
    {
        printf("failed test 6: Step changed in asciiStrForwardIt.\n");
        return;
    }
    
    retcode = mdpl::standardLibrary::String::next(&asciiStrReverseIt);
    if(retcode)
    {
        printf("failed test 6: Error during next on asciiStrReverseIt.\n");
        return;
    }
    if(asciiStrReverseIt.str != asciiStr.s)
    {
        printf("failed test 6: String reference changed in asciiStrReverseIt.\n");
        return;
    }
    if(asciiStrReverseIt.byteIndex != 9)
    {
        printf("failed test 6: New byte index is incorrect for asciiStrReverseIt.\n");
        return;
    }
    if(asciiStrReverseIt.characterIndex != 9)
    {
        printf("failed test 6: New character index is incorrect for asciiStrReverseIt.\n");
        return;
    }
    if(asciiStrReverseIt.step != -1)
    {
        printf("failed test 6: Step changed in asciiStrReverseIt.\n");
        return;
    }
    
    retcode = mdpl::standardLibrary::String::next(&asciiStrDoubleIt);
    if(retcode)
    {
        printf("failed test 6: Error during next on asciiStrDoubleIt.\n");
        return;
    }
    if(asciiStrDoubleIt.str != asciiStr.s)
    {
        printf("failed test 6: String reference changed in asciiStrDoubleIt.\n");
        return;
    }
    if(asciiStrDoubleIt.byteIndex != 2)
    {
        printf("failed test 6: New byte index is incorrect for asciiStrDoubleIt.\n");
        return;
    }
    if(asciiStrDoubleIt.characterIndex != 2)
    {
        printf("failed test 6: New character index is incorrect for asciiStrDoubleIt.\n");
        return;
    }
    if(asciiStrDoubleIt.step != 2)
    {
        printf("failed test 6: Step changed in asciiStrDoubleIt.\n");
        return;
    }

    retcode = mdpl::standardLibrary::String::next(&normaliseStr1It);
    if(retcode)
    {
        printf("failed test 6: Error during next on normaliseStr1It.\n");
        return;
    }
    if(normaliseStr1It.str != normaliseStr1.s)
    {
        printf("failed test 6: String reference changed in normaliseStr1It.\n");
        return;
    }
    if(normaliseStr1It.byteIndex != 2)
    {
        printf("failed test 6: New byte index is incorrect for normaliseStr1It.\n");
        return;
    }
    if(normaliseStr1It.characterIndex != 1)
    {
        printf("failed test 6: New character index is incorrect for normaliseStr1It.\n");
        return;
    }
    if(normaliseStr1It.step != 1)
    {
        printf("failed test 6: Step changed in normaliseStr1It.\n");
        return;
    }

    //temporarily reverse the direction of normaliseStr1It
    normaliseStr1It.step = -1;
    retcode = mdpl::standardLibrary::String::next(&normaliseStr1It);
    if(retcode)
    {
        printf("failed test 6: Error during next on reversed normaliseStr1It.\n");
        return;
    }
    if(normaliseStr1It.str != normaliseStr1.s)
    {
        printf("failed test 6: String reference changed in reversed normaliseStr1It.\n");
        return;
    }
    if(normaliseStr1It.byteIndex != 0)
    {
        printf("failed test 6: New byte index is incorrect for reversed  normaliseStr1It.\n");
        return;
    }
    if(normaliseStr1It.step != -1)
    {
        printf("failed test 6: Step changed in reversed normaliseStr1It.\n");
        return;
    }
    normaliseStr1It.step = 1;

    //test 7: isFinihsed
    retcode = mdpl::standardLibrary::String::isFinished(&asciiStrForwardIt, &result);
    if(retcode)
    {
        printf("failed test 7: Error during isFinished in asciiStrForwardIt.\n");
        return;
    }
    if(result == true)
    {
        printf("failed test 7: isFinished produced wrong result for asciiStrForwardIt.\n");
        return;
    }
    
    retcode = mdpl::standardLibrary::String::isFinished(&asciiStrReverseIt, &result);
    if(retcode)
    {
        printf("failed test 7: Error during isFinished in asciiStrReverseIt.\n");
        return;
    }
    if(result == true)
    {
        printf("failed test 7: isFinished produced wrong result for asciiStrReverseIt.\n");
        return;
    }

    retcode = mdpl::standardLibrary::String::isFinished(&asciiStrDoubleIt, &result);
    if(retcode)
    {
        printf("failed test 7: Error during isFinished in asciiStrDoubleIt.\n");
        return;
    }
    if(result == true)
    {
        printf("failed test 7: isFinished produced wrong result for asciiStrDoubleIt.\n");
        return;
    }

    retcode = mdpl::standardLibrary::String::isFinished(&normaliseStr1It, &result);
    if(retcode)
    {
        printf("failed test 7: Error during isFinished in normaliseStr1It.\n");
        return;
    }
    if(result == true)
    {
        printf("failed test 7: isFinished produced wrong result for normaliseStr1It.\n");
        return;
    }

    mdpl::standardLibrary::String::StringIterator asciiStrFinishedIt = {asciiStr.s, 11, 11, 1};
    retcode = mdpl::standardLibrary::String::isFinished(&asciiStrFinishedIt, &result);
    if(retcode)
    {
        printf("failed test 7: Error during isFinished in asciiStrFinishedIt.\n");
        return;
    }
    if(result == false)
    {
        printf("failed test 7: isFinished produced wrong result for asciiStrFinishedIt.\n");
        return;
    }
    
    //test 8: iterating over entire string
    uint32_t asciiExpectedCharacters[] = {'o', 'r', 'l', 'd'};
    for(size_t i = 0; i < 4; i++)
    {
        retcode = mdpl::standardLibrary::String::isFinished(&asciiStrForwardIt, &result);
        if(retcode)
        {
            printf("failed test 8: Error during isFinished in asciiStrForwardIt.\n");
            return;
        }
        if(result == true)
        {
            printf("failed test 8: isFinished returned true too early for asciiStrForwardIt.\n");
            return;
        }
        retcode = mdpl::standardLibrary::String::getCurrent(&asciiStrForwardIt, &c);
        if(retcode)
        {
            printf("failed test 8: Error during getCurrent in asciiStrForwardIt.\n");
            return;
        }
        if(c.codepoint != asciiExpectedCharacters[i])
        {
            printf("failed test 8: Incorrect character in asciiStrForwardIt.\n");
        }
        retcode = mdpl::standardLibrary::String::next(&asciiStrForwardIt);
        if(retcode)
        {
            printf("failed test 8: Error during next in asciiStrForwardIt.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::isFinished(&asciiStrForwardIt, &result);
    if(retcode)
    {
        printf("failed test 8: Error during isFinished in asciiStrForwardIt.\n");
        return;
    }
    if(result == false)
    {
        printf("failed test 8: isFinished returned incorrect value for asciiStrForwardIt.\n");
        return;
    }

    uint32_t nonAsciiExpectedCharacters[] = {'s', 't', 'r', 'a', 223, 'e', ' ', 948};
    for(size_t i = 0; i < 8; i++)
    {
        retcode = mdpl::standardLibrary::String::isFinished(&nonAsciiStrIt, &result);
        if(retcode)
        {
            printf("failed test 8: Error during isFinished in nonAsciiStrIt.\n");
            return;
        }
        if(result == true)
        {
            printf("failed test 8: isFinished returned true too early for nonAsciiStrIt.\n");
            return;
        }
        retcode = mdpl::standardLibrary::String::getCurrent(&nonAsciiStrIt, &c);
        if(retcode)
        {
            printf("failed test 8: Error during getCurrent in nonAsciiStrIt.\n");
            return;
        }
        if(c.codepoint != nonAsciiExpectedCharacters[i])
        {
            printf("observed=%d expected=%d\n", c.codepoint, nonAsciiExpectedCharacters[i]);
            printf("failed test 8: Incorrect character in nonAsciiStrIt.\n");
        }
        retcode = mdpl::standardLibrary::String::next(&nonAsciiStrIt);
        if(retcode)
        {
            printf("failed test 8: Error during next in nonAsciiStrIt.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::isFinished(&nonAsciiStrIt, &result);
    if(retcode)
    {
        printf("failed test 8: Error during isFinished in nonAsciiStrIt.\n");
        return;
    }
    if(result == true)
    {
        printf("failed test 8: isFinished returned incorrect value for nonAsciiStrIt.\n");
        return;
    }

    //test 9: substrIndex
    size_t originalStartByte = asciiStr.s->startByte;
    size_t originalEndByte = asciiStr.s->endByte;
    mdpl::standardLibrary::String::StringRef asciiIndexSubstr = {};
    retcode = mdpl::standardLibrary::String::substrIndex(asciiStr, &asciiIndexSubstr, 3, 8);
    if(retcode)
    {
        printf("failed test 9: error during substrIndex on asciiStr.\n");
        return;
    }
    if(asciiIndexSubstr.s == asciiStr.s)
    {
        printf("failed test 9: substrIndex coppied by reference not value with asciiIndexsubstr.\n");
        return;
    }
    if(asciiIndexSubstr.s->rawStr != asciiStr.s->rawStr)
    {
        printf("failed test 9: substrIndex did not reference correct raw str asciiIndexsubstr.\n");
        return;
    }
    if(asciiIndexSubstr.s->refCount != 1)
    {
        printf("failed test 9: incorrect ref count for asciiIndexsubstr.\n");
        return;
    }
    if(asciiIndexSubstr.s->numCharacters != 5)
    {
        printf("failed test 9: incorrect num characters for asciiIndexsubstr.\n");
        return;
    }
    if(asciiIndexSubstr.s->startByte != 3)
    {
        printf("failed test 9: incorrect start byte for asciiIndexsubstr.\n");
        return;
    }
    if(asciiIndexSubstr.s->endByte != 8)
    {
        printf("failed test 9: incorrect end byte for asciiIndexsubstr.\n");
        return;
    }
    if((asciiIndexSubstr.s->flagsData & (~(asciiStr.s->flagsData & asciiStr.s->flagsSet))) != 0)
    {
        printf("failed test 9: flags data is incorrect for asciiIndexsubstr.\n");
        return;
    }
    if((asciiIndexSubstr.s->flagsSet & (~(asciiStr.s->flagsData & asciiStr.s->flagsSet))) != 0)
    {
        printf("failed test 9: flags set is incorrect for asciiIndexsubstr.\n");
        return;
    }
    if(asciiIndexSubstr.s->normalisedStr != nullptr)
    {
        printf("failed test 9: normalised str set when it should not be for asciiIndexsubstr.\n");
        return;
    }
    if(asciiStr.s->startByte != originalStartByte)
    {
        printf("failed test 9: original string start byte modified for asciiIndexsubstr.\n");
        return;
    }
    if(asciiStr.s->endByte != originalEndByte)
    {
        printf("failed test 9: original string end byte modified for asciiIndexsubstr.\n");
        return;
    }

    originalStartByte = lowerCaseStr.s->startByte;
    originalEndByte = lowerCaseStr.s->endByte;
    mdpl::standardLibrary::String::StringRef nonAsciiIndexSubstr = {};
    retcode = mdpl::standardLibrary::String::substrIndex(lowerCaseStr, &nonAsciiIndexSubstr, 10, 16);
    if(retcode)
    {
        printf("failed test 9: error during substrIndex on lowerCaseStr.\n");
        return;
    }
    if(nonAsciiIndexSubstr.s == lowerCaseStr.s)
    {
        printf("failed test 9: substrIndex coppied by reference not value with nonAsciiIndexSubstr.\n");
        return;
    }
    if(nonAsciiIndexSubstr.s->rawStr != lowerCaseStr.s->rawStr)
    {
        printf("failed test 9: substrIndex did not reference correct raw str nonAsciiIndexSubstr.\n");
        return;
    }
    if(nonAsciiIndexSubstr.s->refCount != 1)
    {
        printf("failed test 9: incorrect ref count for nonAsciiIndexSubstr.\n");
        return;
    }
    if(nonAsciiIndexSubstr.s->numCharacters != 6)
    {
        printf("failed test 9: incorrect num characters for nonAsciiIndexSubstr.\n");
        return;
    }
    if(nonAsciiIndexSubstr.s->startByte != 10)
    {
        printf("failed test 9: incorrect start byte for nonAsciiIndexSubstr.\n");
        return;
    }
    if(nonAsciiIndexSubstr.s->endByte != 19)
    {
        printf("failed test 9: incorrect end byte for nonAsciiIndexSubstr.\n");
        return;
    }
    if((nonAsciiIndexSubstr.s->flagsData & (~(lowerCaseStr.s->flagsData & lowerCaseStr.s->flagsSet))) != 0)
    {
        printf("failed test 9: flags data is incorrect for nonAsciiIndexSubstr.\n");
        return;
    }
    if((nonAsciiIndexSubstr.s->flagsSet & (~(lowerCaseStr.s->flagsData & lowerCaseStr.s->flagsSet))) != 0)
    {
        printf("failed test 9: flags set is incorrect for nonAsciiIndexSubstr.\n");
        return;
    }
    if(nonAsciiIndexSubstr.s->normalisedStr != nullptr)
    {
        printf("failed test 9: normalised str set when it should not be for nonAsciiIndexSubstr.\n");
        return;
    }
    if(lowerCaseStr.s->startByte != originalStartByte)
    {
        printf("failed test 9: original string start byte modified for nonAsciiIndexSubstr.\n");
        return;
    }
    if(lowerCaseStr.s->endByte != originalEndByte)
    {
        printf("failed test 9: original string end byte modified for nonAsciiIndexSubstr.\n");
        return;
    }

    //test 10: substrIterator
    originalStartByte = asciiStr.s->startByte;
    originalEndByte = asciiStr.s->endByte;
    mdpl::standardLibrary::String::StringRef asciiIteratorSubstr = {};
    mdpl::standardLibrary::String::StringIterator asciiSubstrStartIt = {asciiStr.s, 3, 3, 1};
    mdpl::standardLibrary::String::StringIterator asciiSubstrEndIt = {asciiStr.s, 8, 8, 1};
    retcode = mdpl::standardLibrary::String::substrIterator(asciiStr, &asciiIteratorSubstr, asciiSubstrStartIt, asciiSubstrEndIt);
    if(retcode)
    {
        printf("failed test 10: error during substrIterator on asciiStr.\n");
        return;
    }
    if(asciiIteratorSubstr.s == asciiStr.s)
    {
        printf("failed test 10: substrIterator coppied by reference not value with asciiIteratorsubstr.\n");
        return;
    }
    if(asciiIteratorSubstr.s->rawStr != asciiStr.s->rawStr)
    {
        printf("failed test 10: substrIterator did not reference correct raw str asciiIteratorsubstr.\n");
        return;
    }
    if(asciiIteratorSubstr.s->refCount != 1)
    {
        printf("failed test 10: incorrect ref count for asciiIteratorsubstr.\n");
        return;
    }
    if(asciiIteratorSubstr.s->numCharacters != 5)
    {
        printf("failed test 10: incorrect num characters for asciiIteratorsubstr.\n");
        return;
    }
    if(asciiIteratorSubstr.s->startByte != 3)
    {
        printf("failed test 10: incorrect start byte for asciiIteratorsubstr.\n");
        return;
    }
    if(asciiIteratorSubstr.s->endByte != 8)
    {
        printf("failed test 10: incorrect end byte for asciiIteratorsubstr.\n");
        return;
    }
    if((asciiIteratorSubstr.s->flagsData & (~(asciiStr.s->flagsData & asciiStr.s->flagsSet))) != 0)
    {
        printf("failed test 10: flags data is incorrect for asciiIteratorsubstr.\n");
        return;
    }
    if((asciiIteratorSubstr.s->flagsSet & (~(asciiStr.s->flagsData & asciiStr.s->flagsSet))) != 0)
    {
        printf("failed test 10: flags set is incorrect for asciiIteratorsubstr.\n");
        return;
    }
    if(asciiIteratorSubstr.s->normalisedStr != nullptr)
    {
        printf("failed test 10: normalised str set when it should not be for asciiIteratorsubstr.\n");
        return;
    }
    if(asciiStr.s->startByte != originalStartByte)
    {
        printf("failed test 10: original string start byte modified for asciiIteratorsubstr.\n");
        return;
    }
    if(asciiStr.s->endByte != originalEndByte)
    {
        printf("failed test 10: original string end byte modified for asciiIteratorsubstr.\n");
        return;
    }
    
    originalStartByte = lowerCaseStr.s->startByte;
    originalEndByte = lowerCaseStr.s->endByte;
    mdpl::standardLibrary::String::StringRef nonAsciiIteratorSubstr = {};
    mdpl::standardLibrary::String::StringIterator nonAsciiSubstrStartIt = {lowerCaseStr.s, 10, 10, 1};
    mdpl::standardLibrary::String::StringIterator nonAsciiSubstrEndIt = {lowerCaseStr.s, 19, 16, 1};
    retcode = mdpl::standardLibrary::String::substrIterator(lowerCaseStr, &nonAsciiIteratorSubstr, nonAsciiSubstrStartIt, nonAsciiSubstrEndIt);
    if(retcode)
    {
        printf("failed test 10: error during substrIterator on lowerCaseStr.\n");
        return;
    }
    if(nonAsciiIteratorSubstr.s == lowerCaseStr.s)
    {
        printf("failed test 10: substrIterator coppied by reference not value with nonAsciiIteratorsubstr.\n");
        return;
    }
    if(nonAsciiIteratorSubstr.s->rawStr != lowerCaseStr.s->rawStr)
    {
        printf("failed test 10: substrIterator did not reference correct raw str nonAsciiIteratorsubstr.\n");
        return;
    }
    if(nonAsciiIteratorSubstr.s->refCount != 1)
    {
        printf("failed test 10: incorrect ref count for nonAsciiIteratorsubstr.\n");
        return;
    }
    if(nonAsciiIteratorSubstr.s->numCharacters != 6)
    {
        printf("failed test 10: incorrect num characters for nonAsciiIteratorsubstr.\n");
        return;
    }
    if(nonAsciiIteratorSubstr.s->startByte != 10)
    {
        printf("failed test 10: incorrect start byte for nonAsciiIteratorsubstr.\n");
        return;
    }
    if(nonAsciiIteratorSubstr.s->endByte != 19)
    {
        printf("failed test 10: incorrect end byte for nonAsciiIteratorsubstr.\n");
        return;
    }
    if((nonAsciiIteratorSubstr.s->flagsData & (~(lowerCaseStr.s->flagsData & lowerCaseStr.s->flagsSet))) != 0)
    {
        printf("failed test 10: flags data is incorrect for nonAsciiIteratorsubstr.\n");
        return;
    }
    if((nonAsciiIteratorSubstr.s->flagsSet & (~(lowerCaseStr.s->flagsData & lowerCaseStr.s->flagsSet))) != 0)
    {
        printf("failed test 10: flags set is incorrect for nonAsciiIteratorsubstr.\n");
        return;
    }
    if(nonAsciiIteratorSubstr.s->normalisedStr != nullptr)
    {
        printf("failed test 10: normalised str set when it should not be for nonAsciiIteratorsubstr.\n");
        return;
    }
    if(lowerCaseStr.s->startByte != originalStartByte)
    {
        printf("failed test 10: original string start byte modified for nonAsciiIteratorSubstr.\n");
        return;
    }
    if(lowerCaseStr.s->endByte != originalEndByte)
    {
        printf("failed test 10: original string end byte modified for nonAsciiIteratorSubstr.\n");
        return;
    }

    //test 11: string normalisation
    retcode = mdpl::standardLibrary::String::internal::normaliseString(normaliseStr1.s);
    if(retcode)
    {
        printf("Failed test 11. Error during normalising normalise str 1.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::internal::normaliseString(normaliseStr2.s);
    if(retcode)
    {
        printf("Failed test 11. Error during normalising normalise str 2.\n");
        return;
    }
    if(normaliseStr1.s->normalisedStr->numBytes != normaliseStr2.s->normalisedStr->numBytes)
    {
        printf("Failed test 11. Normalised strings do not match. Different bumber of bytes.\n");
        return;
    }
    for(size_t i = 0; i < normaliseStr1.s->normalisedStr->numBytes; i++)
    {
        if(normaliseStr1.s->normalisedStr->str[i] != normaliseStr2.s->normalisedStr->str[i])
        {
            printf("Failed test 11. Normalised strings do not match.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::internal::normaliseString(asciiStr.s);
    if(retcode)
    {
        printf("Failed test 11. Error during normalising ascii string.\n");
        return;
    }
    if(asciiStr.s->normalisedStr->numBytes != asciiCStrNumBytes)
    {
        printf("Failed test 11. Normalised form of ascii string has the wrong number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < asciiStr.s->normalisedStr->numBytes; i++)
    {
        if(asciiStr.s->normalisedStr->str[i] != asciiCStr[i])
        {
            printf("Failed test 11. Normalised form of ascii string is incorrect.\n");
            return;
        }
    }
    retcode = mdpl::standardLibrary::String::internal::normaliseString(emojiStr.s);
    if(retcode)
    {
        printf("Failed test 11. Error during normalising emoji string.\n");
        return;
    }
    if(emojiStr.s->normalisedStr->numBytes != emojiCStrNumBytes)
    {
        printf("Failed test 11. Normalised form of emoji string has the wrong number of bytes.\n");
        return;
    }
    for(size_t i = 0; i < emojiStr.s->normalisedStr->numBytes; i++)
    {
        if(emojiStr.s->normalisedStr->str[i] != emojiCStr[i])
        {
            printf("Failed test 11. Normalised form of emoji string is incorrect.\n");
            return;
        }
    }

    //test 12: isAscii
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(asciiStr);
    retcode = mdpl::standardLibrary::String::isAscii(asciiStr, &result);
    if(retcode)
    {
        printf("Failed test 12. Error during isAscii call on ascii str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 12. isAscii result is wrong for ascii str.\n");
        return;
    }
    if(((asciiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) == 0) || ((asciiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii) == 0))
    {
        printf("Failed test 12. Flags incorrectly set for ascii str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(emojiStr);
    retcode = mdpl::standardLibrary::String::isAscii(emojiStr, &result);
    if(retcode)
    {
        printf("Failed test 12. Error during isAscii call on emoji str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 12. isAscii result is wrong for emoji str.\n");
        return;
    }
    if(((emojiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) == 0) || ((emojiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii) != 0))
    {
        printf("Failed test 12. Flags incorrectly set for emoji str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(normaliseStr1);
    retcode = mdpl::standardLibrary::String::isAscii(normaliseStr1, &result);
    if(retcode)
    {
        printf("Failed test 12. Error during isAscii call on normalise str 1.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 12. isAscii result is wrong for normalise str 1.\n");
        return;
    }
    if(((normaliseStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) == 0) || ((normaliseStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii) != 0))
    {
        printf("Failed test 12. Flags incorrectly set for normalise str 1.\n");
        return;
    }
    //testing substrings on one function should guarentee it works for all as they share the same internat logic for handling substrings
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(lowerCaseStr);
    retcode = mdpl::standardLibrary::String::isAscii(lowerCaseStr, &result);
    if(retcode)
    {
        printf("Failed test 12. Error during isAscii call on lower case str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 12. isAscii result is wrong for lower case str.\n");
        return;
    }
    if(((lowerCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) == 0) || ((lowerCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii) != 0))
    {
        printf("Failed test 12. Flags incorrectly set for lower case str.\n");
        printf("%d %d\n", (lowerCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii), (lowerCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii));
        printf("%d %d\n", (lowerCaseStr.s->flagsSet), (lowerCaseStr.s->flagsData));
        return;
    }
    mdpl::standardLibrary::String::StringRef lowerCaseStrAsciiSubstr = {};
    if(mdpl::standardLibrary::String::substrIndex(lowerCaseStr, &lowerCaseStrAsciiSubstr, 2, 6))
    {
        printf("Failed test 12. Error during substrIndex.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(lowerCaseStrAsciiSubstr);
    retcode = mdpl::standardLibrary::String::isAscii(lowerCaseStrAsciiSubstr, &result);
    if(retcode)
    {
        printf("Failed test 12. Error during isAscii call on lower case str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 12. isAscii result is wrong for lower case str.\n");
        return;
    }
    if(((lowerCaseStrAsciiSubstr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAscii) == 0) || ((lowerCaseStrAsciiSubstr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAscii) == 0))
    {
        printf("Failed test 12. Flags incorrectly set for lower case str.\n");
        return;
    }
    
    
    //test 13: isValidDecimal
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validDecimalStr);
    retcode = mdpl::standardLibrary::String::isValidDecimal(validDecimalStr, &result);
    if(retcode)
    {
        printf("Failed test 13. Error during isValidDecimal call on valid decimal str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 13. isValidDecimal result is wrong for valid decimal str.\n");
        return;
    }
    if(((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) == 0) || ((validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal) == 0))
    {
        printf("Failed test 13. Flags incorrectly set for valid decimal str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validIntStr);
    retcode = mdpl::standardLibrary::String::isValidDecimal(validIntStr, &result);
    if(retcode)
    {
        printf("Failed test 13. Error during isValidDecimal call on valid int str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 13. isValidDecimal result is wrong for valid int str.\n");
        return;
    }
    if(((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) == 0) || ((validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal) != 0))
    {
        printf("Failed test 13. Flags incorrectly set for valid int str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidIntStr);
    retcode = mdpl::standardLibrary::String::isValidDecimal(invalidIntStr, &result);
    if(retcode)
    {
        printf("Failed test 13. Error during isValidDecimal call on invalid int str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 13. isValidDecimal result is wrong for invalid int str.\n");
        return;
    }
    if(((invalidIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) == 0) || ((invalidIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal) != 0))
    {
        printf("Failed test 13. Flags incorrectly set for invalid int str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validFloatStr);
    retcode = mdpl::standardLibrary::String::isValidDecimal(validFloatStr, &result);
    if(retcode)
    {
        printf("Failed test 13. Error during isValidDecimal call on valid float str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 13. isValidDecimal result is wrong for valid float str.\n");
        return;
    }
    if(((validFloatStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) == 0) || ((validFloatStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal) != 0))
    {
        printf("Failed test 13. Flags incorrectly set for valid float str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidFloatStr1);
    retcode = mdpl::standardLibrary::String::isValidDecimal(invalidFloatStr1, &result);
    if(retcode)
    {
        printf("Failed test 13. Error during isValidDecimal call on invalid float str 1.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 13. isValidDecimal result is wrong for invalid float str 1.\n");
        return;
    }
    if(((invalidFloatStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) == 0) || ((invalidFloatStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal) != 0))
    {
        printf("Failed test 13. Flags incorrectly set for invalid float str 1.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidFloatStr2);
    retcode = mdpl::standardLibrary::String::isValidDecimal(invalidFloatStr2, &result);
    if(retcode)
    {
        printf("Failed test 13. Error during isValidDecimal call on invalid float str 2.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 13. isValidDecimal result is wrong for invalid float str 2.\n");
        return;
    }
    if(((invalidFloatStr2.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidDecimal) == 0) || ((invalidFloatStr2.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidDecimal) != 0))
    {
        printf("Failed test 13. Flags incorrectly set for invalid float str 2.\n");
        return;
    }

    //test 14: isValidInt
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validDecimalStr);
    retcode = mdpl::standardLibrary::String::isValidInt(validDecimalStr, &result);
    if(retcode)
    {
        printf("Failed test 14. Error during isValidInt call on valid decimal str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 14. isValidInt result is wrong for valid decimal str.\n");
        return;
    }
    if(((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0) || ((validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0))
    {
        printf("Failed test 14. Flags incorrectly set for valid decimal str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validIntStr);
    retcode = mdpl::standardLibrary::String::isValidInt(validIntStr, &result);
    if(retcode)
    {
        printf("Failed test 14. Error during isValidInt call on valid int str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 14. isValidInt result is wrong for valid int str.\n");
        return;
    }
    if(((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0) || ((validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0))
    {
        printf("Failed test 14. Flags incorrectly set for valid int str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidIntStr);
    retcode = mdpl::standardLibrary::String::isValidInt(invalidIntStr, &result);
    if(retcode)
    {
        printf("Failed test 14. Error during isValidInt call on invalid int str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 14. isValidInt result is wrong for invalid int str.\n");
        return;
    }
    if(((invalidIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0) || ((invalidIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt) != 0))
    {
        printf("Failed test 14. Flags incorrectly set for invalid int str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validFloatStr);
    retcode = mdpl::standardLibrary::String::isValidInt(validFloatStr, &result);
    if(retcode)
    {
        printf("Failed test 14. Error during isValidInt call on valid float str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 14. isValidInt result is wrong for valid float str.\n");
        return;
    }
    if(((validFloatStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0) || ((validFloatStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt) != 0))
    {
        printf("Failed test 14. Flags incorrectly set for valid float str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidFloatStr1);
    retcode = mdpl::standardLibrary::String::isValidInt(invalidFloatStr1, &result);
    if(retcode)
    {
        printf("Failed test 14. Error during isValidInt call on invalid float str 1.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 14. isValidInt result is wrong for invalid float str 1.\n");
        return;
    }
    if(((invalidFloatStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0) || ((invalidFloatStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt) != 0))
    {
        printf("Failed test 14. Flags incorrectly set for invalid float str 1.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidFloatStr2);
    retcode = mdpl::standardLibrary::String::isValidInt(invalidFloatStr2, &result);
    if(retcode)
    {
        printf("Failed test 14. Error during isValidInt call on invalid float str 2.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 14. isValidInt result is wrong for invalid float str 2.\n");
        return;
    }
    if(((invalidFloatStr2.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidInt) == 0) || ((invalidFloatStr2.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidInt) != 0))
    {
        printf("Failed test 14. Flags incorrectly set for invalid float str 2.\n");
        return;
    }

    //test 15: isValidFloat
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validDecimalStr);
    retcode = mdpl::standardLibrary::String::isValidFloat(validDecimalStr, &result);
    if(retcode)
    {
        printf("Failed test 15. Error during isValidFloat call on valid decimal str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 15. isValidFloat result is wrong for valid decimal str.\n");
        return;
    }
    if(((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0) || ((validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0))
    {
        printf("Failed test 15. Flags incorrectly set for valid decimal str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validIntStr);
    retcode = mdpl::standardLibrary::String::isValidFloat(validIntStr, &result);
    if(retcode)
    {
        printf("Failed test 15. Error during isValidFloat call on valid int str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 15. isValidFloat result is wrong for valid int str.\n");
        return;
    }
    if(((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0) || ((validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0))
    {
        printf("Failed test 15. Flags incorrectly set for valid int str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidIntStr);
    retcode = mdpl::standardLibrary::String::isValidFloat(invalidIntStr, &result);
    if(retcode)
    {
        printf("Failed test 15. Error during isValidFloat call on invalid int str.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 15. isValidFloat result is wrong for invalid int str.\n");
        return;
    }
    if(((invalidIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0) || ((invalidIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat) != 0))
    {
        printf("Failed test 15. Flags incorrectly set for invalid int str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validFloatStr);
    retcode = mdpl::standardLibrary::String::isValidFloat(validFloatStr, &result);
    if(retcode)
    {
        printf("Failed test 15. Error during isValidFloat call on valid float str.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 15. isValidFloat result is wrong for valid float str.\n");
        return;
    }
    if(((validFloatStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0) || ((validFloatStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0))
    {
        printf("Failed test 15. Flags incorrectly set for valid float str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidFloatStr1);
    retcode = mdpl::standardLibrary::String::isValidFloat(invalidFloatStr1, &result);
    if(retcode)
    {
        printf("Failed test 15. Error during isValidFloat call on invalid float str 1.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 15. isValidFloat result is wrong for invalid float str 1.\n");
        return;
    }
    if(((invalidFloatStr1.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0) || ((invalidFloatStr1.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat) != 0))
    {
        printf("Failed test 15. Flags incorrectly set for invalid float str 1.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(invalidFloatStr2);
    retcode = mdpl::standardLibrary::String::isValidFloat(invalidFloatStr2, &result);
    if(retcode)
    {
        printf("Failed test 15. Error during isValidFloat call on invalid float str 2.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 15. isValidFloat result is wrong for invalid float str 2.\n");
        return;
    }
    if(((invalidFloatStr2.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isValidFloat) == 0) || ((invalidFloatStr2.s->flagsData & mdpl::standardLibrary::String::StringFlags::isValidFloat) != 0))
    {
        printf("Failed test 15. Flags incorrectly set for invalid float str 2.\n");
        return;
    }

    //test 16: isLower
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(lowerCaseStr);
    retcode = mdpl::standardLibrary::String::isLower(lowerCaseStr, &result);
    if(result == false)
    {
        printf("Failed test 16. isLower is incorrect for lower case str.\n");
        return;
    }
    if(((lowerCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isLower) == 0) || ((lowerCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isLower) == 0))
    {
        printf("Failed test 16. isLower incorrectly set flags for lower case str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(upperCaseStr);
    retcode = mdpl::standardLibrary::String::isLower(upperCaseStr, &result);
    if(result == true)
    {
        printf("Failed test 16. isLower is incorrect for upper case str.\n");
        return;
    }
    if(((upperCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isLower) == 0) || ((upperCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isLower) != 0))
    {
        printf("Failed test 16. isLower incorrectly set flags for upper case str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(mixedCaseStr);
    retcode = mdpl::standardLibrary::String::isLower(mixedCaseStr, &result);
    if(result == true)
    {
        printf("Failed test 16. isLower is incorrect for mixed case str.\n");
        return;
    }
    if(((mixedCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isLower) == 0) || ((mixedCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isLower) != 0))
    {
        printf("Failed test 16. isLower incorrectly set flags for mixed case str.\n");
        return;
    }

    //test 17: isUpper
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(lowerCaseStr);
    retcode = mdpl::standardLibrary::String::isUpper(lowerCaseStr, &result);
    if(result == true)
    {
        printf("Failed test 17. isUpper is incorrect for lower case str.\n");
        return;
    }
    if(((lowerCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isUpper) == 0) || ((lowerCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isUpper) != 0))
    {
        printf("Failed test 17. isUpper incorrectly set flags for lower case str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(upperCaseStr);
    retcode = mdpl::standardLibrary::String::isUpper(upperCaseStr, &result);
    if(result == false)
    {
        printf("Failed test 17. isUpper is incorrect for upper case str.\n");
        return;
    }
    if(((upperCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isUpper) == 0) || ((upperCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isUpper) == 0))
    {
        printf("Failed test 17. isUpper incorrectly set flags for upper case str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(mixedCaseStr);
    retcode = mdpl::standardLibrary::String::isUpper(mixedCaseStr, &result);
    if(result == true)
    {
        printf("Failed test 17. isUpper is incorrect for mixed case str.\n");
        return;
    }
    if(((mixedCaseStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isUpper) == 0) || ((mixedCaseStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isUpper) != 0))
    {
        printf("Failed test 17. isUpper incorrectly set flags for mixed case str.\n");
        return;
    }

    //test 18: isWhitespace
    //the following list of characters is sourced from https://en.wikipedia.org/wiki/Whitespace_character
    //as some of these characters are considered non-stnadard some systems may automatically remove them. For example \r. To guarentee this does not break the test the string is created at runtime. 
    utf8proc_int32_t whiteSpaceCodepoints[] = {0x0009, 0x000A, 0x000C, 0x0020, 0x1680, 0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006, 0x2007, 0x2008, 0x2009, 0x2008, 0x2009, 0x200A, 0x2028, 0x205F, 0x3000};
    utf8proc_ssize_t whiteSpaceCodepointsLength = sizeof(whiteSpaceCodepoints) / sizeof(utf8proc_int32_t);
    utf8proc_ssize_t utf8proc_result = utf8proc_reencode(whiteSpaceCodepoints, whiteSpaceCodepointsLength, static_cast<utf8proc_option_t>(0));
    if(utf8proc_result < 0) { printf("Something went wrong during setup for test 18. %s.\n", utf8proc_errmsg(utf8proc_result)); }
    mdpl::standardLibrary::String::StringRef whitespaceStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&whitespaceStr, reinterpret_cast<const char*>(whiteSpaceCodepoints), strlen(reinterpret_cast<const char*>(whiteSpaceCodepoints)), 19);
    if(retcode) { printf("Failed test 18. Error during constructing whitespace str.\n"); return; }

    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(whitespaceStr);
    retcode = mdpl::standardLibrary::String::isWhiteSpace(whitespaceStr, &result);
    if(result == false)
    {
        printf("Failed test 18. isWhiteSpace is incorrect for white space str.\n");
        return;
    }
    if(((whitespaceStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isWhiteSpace) == 0) || ((whitespaceStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isWhiteSpace) == 0))
    {
        printf("Failed test 18. isWhiteSpace incorrectly set flags for white space str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(asciiStr);
    retcode = mdpl::standardLibrary::String::isWhiteSpace(asciiStr, &result);
    if(result == true)
    {
        printf("Failed test 18. isWhiteSpace is incorrect for ascii str.\n");
        return;
    }
    if(((asciiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isWhiteSpace) == 0) || ((asciiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isWhiteSpace) != 0))
    {
        printf("Failed test 18. isWhiteSpace incorrectly set flags for ascii str.\n");
        return;
    }

    //test 19: isPrintable
    //these charcters cannot be printed so they have to be stored as hex
    utf8proc_int32_t unprintableCodepoints[] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0888, 0x0889, 0x1328, 0x1515};
    utf8proc_ssize_t unprintableCodepointsLength = sizeof(unprintableCodepoints) / sizeof(utf8proc_int32_t);
    utf8proc_result = utf8proc_reencode(unprintableCodepoints, unprintableCodepointsLength, static_cast<utf8proc_option_t>(0));
    if(utf8proc_result < 0) { printf("Something went wrong during setup for test 19. %s.\n", utf8proc_errmsg(utf8proc_result)); }
    mdpl::standardLibrary::String::StringRef unprintableStr = {};
    retcode = mdpl::standardLibrary::String::createStringRefFromCStr(&unprintableStr, reinterpret_cast<const char*>(unprintableCodepoints), strlen(reinterpret_cast<const char*>(unprintableCodepoints)), 19);
    if(retcode) { printf("Failed test 18. Error during constructing whitespace str.\n"); return; }

    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(unprintableStr);
    retcode = mdpl::standardLibrary::String::isPrintable(unprintableStr, &result);
    if(result == true)
    {
        printf("Failed test 19. isPrintable is incorrect for white space str.\n");
        return;
    }
    if(((unprintableStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isPrintable) == 0) || ((unprintableStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isPrintable) != 0))
    {
        printf("Failed test 19. isPrintable incorrectly set flags for white space str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(asciiStr);
    retcode = mdpl::standardLibrary::String::isPrintable(asciiStr, &result);
    if(result == false)
    {
        printf("Failed test 19. isPrintable is incorrect for ascii str.\n");
        return;
    }
    if(((asciiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isPrintable) == 0) || ((asciiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isPrintable) == 0))
    {
        printf("Failed test 19. isPrintable incorrectly set flags for ascii str.\n");
        return;
    }

    //test 20: isAlpha
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(asciiStr);
    retcode = mdpl::standardLibrary::String::isAlpha(asciiStr, &result);
    if(result == true)
    {
        printf("Failed test 20. isAlpha is incorrect for ascii str.\n");
        return;
    }
    if(((asciiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlpha) == 0) || ((asciiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlpha) != 0))
    {
        printf("Failed test 20. isAlpha incorrectly set flags for ascii str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(alphaStr);
    retcode = mdpl::standardLibrary::String::isAlpha(alphaStr, &result);
    if(result == false)
    {
        printf("Failed test 20. isAlpha is incorrect for alpha str.\n");
        return;
    }
    if(((alphaStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlpha) == 0) || ((alphaStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlpha) == 0))
    {
        printf("Failed test 20. isAlpha incorrectly set flags for alpha str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(alphaNumStr);
    retcode = mdpl::standardLibrary::String::isAlpha(alphaNumStr, &result);
    if(result == true)
    {
        printf("Failed test 20. isAlpha is incorrect for alpha numeric str.\n");
        return;
    }
    if(((alphaNumStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlpha) == 0) || ((alphaNumStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlpha) != 0))
    {
        printf("Failed test 20. isAlpha incorrectly set flags for alpha numeric str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validDecimalStr);
    retcode = mdpl::standardLibrary::String::isAlpha(validDecimalStr, &result);
    if(result == true)
    {
        printf("Failed test 20. isAlpha is incorrect for valid decimal str.\n");
        return;
    }
    if(((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlpha) == 0) || ((validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlpha) != 0))
    {
        printf("Failed test 20. isAlpha incorrectly set flags for valid decimal str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validIntStr);
    retcode = mdpl::standardLibrary::String::isAlpha(validIntStr, &result);
    if(result == true)
    {
        printf("Failed test 20. isAlpha is incorrect for valid int str.\n");
        return;
    }
    if(((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlpha) == 0) || ((validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlpha) != 0))
    {
        printf("Failed test 20. isAlpha incorrectly set flags for valid int str.\n");
        return;
    }

    //test 21: isAlphaNumeric
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(asciiStr);
    retcode = mdpl::standardLibrary::String::isAlphaNumeric(asciiStr, &result);
    if(result == true)
    {
        printf("Failed test 21. isAlphaNumeric is incorrect for ascii str.\n");
        return;
    }
    if(((asciiStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0) || ((asciiStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) != 0))
    {
        printf("Failed test 21. isAlphaNumeric incorrectly set flags for ascii str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(alphaStr);
    retcode = mdpl::standardLibrary::String::isAlphaNumeric(alphaStr, &result);
    if(result == false)
    {
        printf("Failed test 21. isAlphaNumeric is incorrect for alpha str.\n");
        return;
    }
    if(((alphaStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0) || ((alphaStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0))
    {
        printf("Failed test 21. isAlphaNumeric incorrectly set flags for alpha str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(alphaNumStr);
    retcode = mdpl::standardLibrary::String::isAlphaNumeric(alphaNumStr, &result);
    if(result == false)
    {
        printf("Failed test 21. isAlphaNumeric is incorrect for alpha numeric str.\n");
        return;
    }
    if(((alphaNumStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0) || ((alphaNumStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0))
    {
        printf("Failed test 21. isAlphaNumeric incorrectly set flags for alpha numeric str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validDecimalStr);
    retcode = mdpl::standardLibrary::String::isAlphaNumeric(validDecimalStr, &result);
    if(result == false)
    {
        printf("Failed test 21. isAlphaNumeric is incorrect for valid decimal str.\n");
        return;
    }
    if(((validDecimalStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0) || ((validDecimalStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0))
    {
        printf("Failed test 21. isAlphaNumeric incorrectly set flags for valid decimal str.\n");
        return;
    }
    MDPL_STRING_TESTER_DISABLE_ASCII_OPTIMISATION(validIntStr);
    retcode = mdpl::standardLibrary::String::isAlphaNumeric(validIntStr, &result);
    if(result == true)
    {
        printf("Failed test 20. isAlphaNumeric is incorrect for valid int str.\n");
        return;
    }
    if(((validIntStr.s->flagsSet & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) == 0) || ((validIntStr.s->flagsData & mdpl::standardLibrary::String::StringFlags::isAlphaNumeric) != 0))
    {
        printf("Failed test 20. isAlphaNumeric incorrectly set flags for valid int str.\n");
        return;
    }

    //test 22: startsWith
    retcode = mdpl::standardLibrary::String::startsWith(lowerCaseStr, lowerCasePrefixStr, &result);
    if(retcode)
    {
        printf("Failed test 22. error during startsWith on lowerCaseStr with lowerCasePrefixStr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 22. lowerCaseStr should start with lowerCasePrefixStr but does not.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::startsWith(lowerCaseStr, upperCasePrefixStr, &result);
    if(retcode)
    {
        printf("Failed test 22. error during startsWith on lowerCaseStr with upperCasePrefixStr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 22. lowerCaseStr should not start with upperCasePrefixStr but does.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::startsWith(upperCaseStr, lowerCasePrefixStr, &result);
    if(retcode)
    {
        printf("Failed test 22. error during startsWith on upperCaseStr with lowerCasePrefixStr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 22. upperCaseStr should not start with lowerCasePrefixStr but does.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::startsWith(upperCaseStr, upperCasePrefixStr, &result);
    if(retcode)
    {
        printf("Failed test 22. error during startsWith on upperCaseStr with upperCasePrefixStr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 22. upperCaseStr should start with upperCasePrefixStr but does not.\n");
        return;
    }

    //test 23: endsWith
    retcode = mdpl::standardLibrary::String::endsWith(lowerCaseStr, lowerCaseSuffixStr, &result);
    if(retcode)
    {
        printf("Failed test 23. error during endsWith on lowerCaseStr with lowerCaseSuffixStr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 23. lowerCaseStr should start with lowerCaseSuffixStr but does not.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::endsWith(lowerCaseStr, upperCaseSuffixStr, &result);
    if(retcode)
    {
        printf("Failed test 23. error during endsWith on lowerCaseStr with upperCaseSuffixStr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 23. lowerCaseStr should not start with upperCaseSuffixStr but does.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::endsWith(upperCaseStr, lowerCaseSuffixStr, &result);
    if(retcode)
    {
        printf("Failed test 23. error during endsWith on upperCaseStr with lowerCaseSuffixStr.\n");
        return;
    }
    if(result == true)
    {
        return;
    }
    retcode = mdpl::standardLibrary::String::endsWith(upperCaseStr, upperCaseSuffixStr, &result);
    if(retcode)
    {
        printf("Failed test 23. error during endsWith on upperCaseStr with upperCaseSuffixStr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 23. upperCaseStr should start with upperCaseSuffixStr but does not.\n");
        return;
    }

    //setup for test 24-39
    mdpl::standardLibrary::String::StringIterator it = {};

    //test 24: frontForwardsIterator
    retcode = mdpl::standardLibrary::String::frontForwardsIterator(lowerCaseStr, &it);
    if(retcode)
    {
        printf("Failed test 24. error during frontForwardsIterator.\n");
        return;
    }
    if(it.str != lowerCaseStr.s)
    {
        printf("Failed test 24. iterator's string pointer does not match string's string pointer.\n");
        return;
    }
    if(it.byteIndex != 0)
    {
        printf("Failed test 24. byteIndex incorect.\n");
        return;
    }
    if(it.characterIndex != 0)
    {
        printf("Failed test 24. characterIndex incorect.\n");
        return;
    }
    if(it.step != 1)
    {
        printf("Failed test 24. step incorect.\n");
        return;
    }

    //test 25: backReverseIterator
    retcode = mdpl::standardLibrary::String::backReverseIterator(lowerCaseStr, &it);
    if(retcode)
    {
        printf("Failed test 25. error during backReverseIterator.\n");
        return;
    }
    if(it.str != lowerCaseStr.s)
    {
        printf("Failed test 25. iterator's string pointer does not match string's string pointer.\n");
        return;
    }
    if(it.byteIndex != 25)
    {
        printf("Failed test 25. byteIndex incorect.\n");
        return;
    }
    if(it.characterIndex != 19)
    {
        printf("Failed test 25. characterIndex incorect.\n");
        return;
    }
    if(it.step != -1)
    {
        printf("Failed test 25. step incorect.\n");
        return;
    }

    //setups for test 26-41
    mdpl::standardLibrary::String::Character lowerCaseAsciiChr = {'h'};
    mdpl::standardLibrary::String::Character upperCaseAsciiChr = {'H'};
    mdpl::standardLibrary::String::Character lowerCaseUnicodeChr = {954};
    mdpl::standardLibrary::String::Character upperCaseUnicodeChr = {922};
    mdpl::standardLibrary::String::Character numberChr = {'1'};
    mdpl::standardLibrary::String::Character dashChr = {'-'};
    mdpl::standardLibrary::String::Character dotChr = {'.'};
    mdpl::standardLibrary::String::Character spaceChr = {' '};
    mdpl::standardLibrary::String::Character newLineChr = {'\n'};
    mdpl::standardLibrary::String::Character nullChr = {'\0'};

    //test 26: isLowerChr
    retcode = mdpl::standardLibrary::String::isLowerChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 26. error durng isLowerChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 26. isLowerChr incorrect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isLowerChr(&upperCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 26. error durng isLowerChr with upperCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 26. isLowerChr incorrect for upperCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isLowerChr(&lowerCaseUnicodeChr, &result);
    if(retcode)
    {
        printf("Failed test 26. error durng isLowerChr with lowerCaseUnicodeChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 26. isLowerChr incorrect for lowerCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isLowerChr(&upperCaseUnicodeChr, &result);
    if(retcode)
    {
        printf("Failed test 26. error durng isLowerChr with upperCaseUnicodeChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 26. isUpperChr isLowerChr for upperCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isLowerChr(&dashChr, &result);
    if(retcode)
    {
        printf("Failed test 26. error durng isLowerChr with dashChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 26. isUpperChr isLowerChr for dashChr.\n");
        return;
    }

    //test 27: isUpperChr
    retcode = mdpl::standardLibrary::String::isUpperChr(&upperCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 27. error durng isUpperChr with upperCaseAsciiChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 27. isUpperChr incorrect for upperCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isUpperChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 27. error durng isUpperChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 27. isUpperChr incorrect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isUpperChr(&upperCaseUnicodeChr, &result);
    if(retcode)
    {
        printf("Failed test 27. error durng isUpperChr with upperCaseUnicodeChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 27. isUpperChr incorrect for upperCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isUpperChr(&lowerCaseUnicodeChr, &result);
    if(retcode)
    {
        printf("Failed test 27. error durng isUpperChr with lowerCaseUnicodeChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 27. isUpperChr incorrect for lowerCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isUpperChr(&dashChr, &result);
    if(retcode)
    {
        printf("Failed test 27. error durng isUpperChr with dashChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 27. isUpperChr incorrect for dashChr.\n");
        return;
    }
    
    //test 28: isWhiteSpaceChr
    retcode = mdpl::standardLibrary::String::isWhiteSpaceChr(&spaceChr, &result);
    if(retcode)
    {
        printf("Failed test 28. error durng isWhiteSpaceChr with spaceChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 28. isWhiteSpaceChr incorrect for spaceChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isWhiteSpaceChr(&newLineChr, &result);
    if(retcode)
    {
        printf("Failed test 28. error durng isWhiteSpaceChr with newLineChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 28. isWhiteSpaceChr incorrect for newLineChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isWhiteSpaceChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 28. error durng isWhiteSpaceChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 28. isWhiteSpaceChr incorect for lowerCaseAsciiChr.\n");
        return;
    }

    //test 29: isPrintableChr
    retcode = mdpl::standardLibrary::String::isPrintableChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 29. error durng isPrintableChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 29. isPrintableChr incorect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isPrintableChr(&nullChr, &result);
    if(retcode)
    {
        printf("Failed test 29. error durng isPrintableChr with nullChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 29. isPrintableChr incorect for nullChr.\n");
        return;
    }

    //test 30: isAsciiChr
    retcode = mdpl::standardLibrary::String::isAsciiChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 30. error durng isAsciiChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 30. isAsciiChr incorect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isAsciiChr(&lowerCaseUnicodeChr, &result);
    if(retcode)
    {
        printf("Failed test 30. error durng isAsciiChr with lowerCaseUnicodeChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 30. isAsciiChr incorect for lowerCaseUnicodeChr.\n");
        return;
    }

    //test 31: isDecimalChr
    retcode = mdpl::standardLibrary::String::isDecimalChr(&numberChr, &result);
    if(retcode)
    {
        printf("Failed test 31. error durng isDecimalChr with numberChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 31. isDecimalChr incorect for numberChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isDecimalChr(&dashChr, &result);
    if(retcode)
    {
        printf("Failed test 31. error durng isDecimalChr with dashChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 31. isDecimalChr incorect for dashChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isDecimalChr(&dotChr, &result);
    if(retcode)
    {
        printf("Failed test 31. error durng isDecimalChr with dotChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 31. isDecimalChr incorect for dotChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isDecimalChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 31. error durng isDecimalChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 31. isDecimalChr incorect for lowerCaseAsciiChr.\n");
        return;
    }

    //test 32: isIntChr
    retcode = mdpl::standardLibrary::String::isIntChr(&numberChr, &result);
    if(retcode)
    {
        printf("Failed test 32. error durng isIntChr with numberChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 32. isIntChr incorect for numberChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isIntChr(&dashChr, &result);
    if(retcode)
    {
        printf("Failed test 32. error durng isIntChr with dashChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 32. isIntChr incorect for dashChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isIntChr(&dotChr, &result);
    if(retcode)
    {
        printf("Failed test 32. error durng isIntChr with dotChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 32. isIntChr incorect for dotChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isIntChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 32. error durng isIntChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 32. isIntChr incorect for lowerCaseAsciiChr.\n");
        return;
    }

    //test 33: isFloatChr
    retcode = mdpl::standardLibrary::String::isFloatChr(&numberChr, &result);
    if(retcode)
    {
        printf("Failed test 33. error durng isFloatChr with numberChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 33. isFloatChr incorect for numberChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isFloatChr(&dashChr, &result);
    if(retcode)
    {
        printf("Failed test 33. error durng isFloatChr with dashChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 33. isFloatChr incorect for dashChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isFloatChr(&dotChr, &result);
    if(retcode)
    {
        printf("Failed test 33. error durng isFloatChr with dotChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 33. isFloatChr incorect for dotChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isFloatChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 33. error durng isFloatChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 33. isFloatChr incorect for lowerCaseAsciiChr.\n");
        return;
    }

    //test 34: isAlphaChr
    retcode = mdpl::standardLibrary::String::isAlphaChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 34. error durng isAlphaChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 34. isAlphaChr incorect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isAlphaChr(&numberChr, &result);
    if(retcode)
    {
        printf("Failed test 34. error durng isAlphaChr with numberChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 34. isAlphaChr incorect for numberChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isAlphaChr(&dashChr, &result);
    if(retcode)
    {
        printf("Failed test 34. error durng isAlphaChr with dashChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 34. isAlphaChr incorect for dashChr.\n");
        return;
    }

    //test 35: isAlphaNumericChr
    retcode = mdpl::standardLibrary::String::isAlphaNumericChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 35. error durng isAlphaNumericChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 35. isAlphaNumericChr incorect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isAlphaNumericChr(&numberChr, &result);
    if(retcode)
    {
        printf("Failed test 35. error durng isAlphaNumericChr with numberChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 35. isAlphaNumericChr incorect for numberChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isAlphaNumericChr(&dashChr, &result);
    if(retcode)
    {
        printf("Failed test 35. error durng isAlphaNumericChr with dashChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 35. isAlphaNumericChr incorect for dashChr.\n");
        return;
    }

    //test 36: isNewLineChr
    retcode = mdpl::standardLibrary::String::isNewLineChr(&newLineChr, &result);
    if(retcode)
    {
        printf("Failed test 36. error durng isNewLineChr with newLineChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 36. isNewLineChr incorect for newLineChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isNewLineChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 36. error durng isNewLineChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 36. isNewLineChr incorect for lowerCaseAsciiChr.\n");
        return;
    }

    //test 37: isNullChr
    retcode = mdpl::standardLibrary::String::isNullChr(&nullChr, &result);
    if(retcode)
    {
        printf("Failed test 37. error durng isNullChr with nullChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 37. isNullChr incorect for nullChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::isNullChr(&lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 37. error durng isNullChr with lowerCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 37. isNullChr incorect for lowerCaseAsciiChr.\n");
        return;
    }

    //test 38: valueEqualityChrChr
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&lowerCaseAsciiChr, &lowerCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 38. error durng valueEqualityChrChr with lowerCaseAsciiChr and lowerCaseAsciiChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 38. valueEqualityChrChr incorect for lowerCaseAsciiChr and lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&lowerCaseAsciiChr, &upperCaseAsciiChr, &result);
    if(retcode)
    {
        printf("Failed test 38. error durng valueEqualityChrChr with lowerCaseAsciiChr and upperCaseAsciiChr.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 38. valueEqualityChrChr incorect for lowerCaseAsciiChr and upperCaseAsciiChr.\n");
        return;
    }

    //test 39: valueEqualityChrUnicode
    retcode = mdpl::standardLibrary::String::valueEqualityChrUnicode(&lowerCaseAsciiChr, 'h', &result);
    if(retcode)
    {
        printf("Failed test 39. error durng valueEqualityChrUnicode with lowerCaseAsciiChr and \'h\'.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 39. valueEqualityChrUnicode incorect for lowerCaseAsciiChr and \'h\'.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrUnicode(&lowerCaseAsciiChr, 'H', &result);
    if(retcode)
    {
        printf("Failed test 39. error durng valueEqualityChrUnicode with lowerCaseAsciiChr and \'H\'.\n");
        return;
    }
    if(result == true)
    {
        printf("Failed test 39. valueEqualityChrUnicode incorect for lowerCaseAsciiChr and \'H\'.\n");
        return;
    }
    
    //setup for test 40 and 41
    mdpl::standardLibrary::String::Character newChr;

    //test 40: toLowerChr
    retcode = mdpl::standardLibrary::String::toLowerChr(&lowerCaseAsciiChr, &newChr);
    if(retcode)
    {
        printf("Failed test 40. error durng toLowerChr with lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&lowerCaseAsciiChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 40. error durng valueEqualityChrChr with lowerCaseAsciiChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 40. toLowerChr incorect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::toLowerChr(&lowerCaseAsciiChr, &newChr);
    if(retcode)
    {
        printf("Failed test 40. error durng toLowerChr with lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&lowerCaseAsciiChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 40. error durng valueEqualityChrChr with lowerCaseAsciiChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 40. toLowerChr incorect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::toLowerChr(&lowerCaseUnicodeChr, &newChr);
    if(retcode)
    {
        printf("Failed test 40. error durng toLowerChr with lowerCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&lowerCaseUnicodeChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 40. error durng valueEqualityChrChr with lowerCaseUnicodeChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 40. toLowerChr incorect for lowerCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::toLowerChr(&lowerCaseUnicodeChr, &newChr);
    if(retcode)
    {
        printf("Failed test 40. error durng toLowerChr with lowerCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&lowerCaseUnicodeChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 40. error durng valueEqualityChrChr with lowerCaseUnicodeChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 40. toLowerChr incorect for lowerCaseUnicodeChr.\n");
        return;
    }

    //test 41: toUpperChr
    retcode = mdpl::standardLibrary::String::toUpperChr(&lowerCaseAsciiChr, &newChr);
    if(retcode)
    {
        printf("Failed test 41. error durng toUpperChr with lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&upperCaseAsciiChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 41. error durng valueEqualityChrChr with upperCaseAsciiChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 41. toUpperChr incorect for lowerCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::toUpperChr(&upperCaseAsciiChr, &newChr);
    if(retcode)
    {
        printf("Failed test 41. error durng toUpperChr with upperCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&upperCaseAsciiChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 41. error durng valueEqualityChrChr with upperCaseAsciiChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 41. toUpperChr incorect for upperCaseAsciiChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::toUpperChr(&lowerCaseUnicodeChr, &newChr);
    if(retcode)
    {
        printf("Failed test 41. error durng toUpperChr with lowerCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&upperCaseUnicodeChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 41. error durng valueEqualityChrChr with upperCaseUnicodeChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 41. toUpperChr incorect for lowerCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::toUpperChr(&upperCaseUnicodeChr, &newChr);
    if(retcode)
    {
        printf("Failed test 41. error durng toUpperChr with upperCaseUnicodeChr.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::valueEqualityChrChr(&upperCaseUnicodeChr, &newChr, &result);
    if(retcode)
    {
        printf("Failed test 41. error durng valueEqualityChrChr with upperCaseUnicodeChr and newChr.\n");
        return;
    }
    if(result == false)
    {
        printf("Failed test 41. toUpperChr incorect for upperCaseUnicodeChr.\n");
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
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(upperCaseStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of upper case string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(mixedCaseStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of mixed case string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(whitespaceStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of white space string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(unprintableStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of white space string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(alphaStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of alpha string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(alphaNumStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of alpha numeric string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(asciiIndexSubstr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of ascii index substring.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(nonAsciiIndexSubstr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of ascii index substring.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(asciiIteratorSubstr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of ascii index substring.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(nonAsciiIteratorSubstr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of non ascii index substring.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(lowerCaseStrAsciiSubstr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of non ascii index substring.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(lowerCasePrefixStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of lower case prefix string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(upperCasePrefixStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of upper case prefix string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(lowerCaseSuffixStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of lower case suffix string.\n");
        return;
    }
    retcode = mdpl::standardLibrary::String::destroyStringRef(upperCaseSuffixStr);
    if(retcode)
    {
        printf("Failed final test. Error during deconstruction of upper case suffix string.\n");
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