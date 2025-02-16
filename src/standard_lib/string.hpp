#ifndef STRING_HEADER_GUARD
#define STRING_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "../runtime_lib/allocator.hpp"
#include "../../vendor/utf8proc/utf8proc.h"

namespace mdpl
{
    namespace standardLibrary
    {
        namespace String
        {
            /*
            namespace StringFlags
            {
                uint32_t isLower        = 1;
                uint32_t isUpper        = 2;
                uint32_t isWhiteSpace   = 4;
                uint32_t isPrintable    = 8;
                uint32_t isAscii        = 16;
                uint32_t isDecimal      = 32;
                uint32_t isInt          = 64;
                uint32_t isFloat        = 128;
                uint32_t isAlpha        = 256;
                uint32_t isAlphaNumeric = 512;
            };
            */
            struct RawString
            {
                size_t refCount;
                const size_t numBytes;
                const char str[1];
            };
            struct String
            {
                size_t refCount;
                size_t numCharacters;
                size_t startByte;
                size_t endByte; //up to but not including
                uint32_t flagsData;
                uint32_t flagsSet;
                RawString* const rawStr;
                RawString* normalisedStr;
            };
            struct StringRef
            {
                String* const s;
            };
            struct Character
            {
                size_t numBytes;
                char str[4];
            };
            struct StringForwardsIterator
            {
                const String* const str;
                size_t codePointIndex;
            };

            const size_t npos = static_cast<size_t>(-1);

            //================ String functions ================
            
            int count(const String* const str, size_t* ocurances);
            int findIndex(const String* const str, const String* const target, size_t* characterIndex);
            int rFindIndex(const String* const str, const String* const target, size_t* characterIndex);
            int findForwardsIterator(const String* const str, const String* const target, size_t* StringForwardsIterator);
            int rFindForwardsIterator(const String* const str, const String* const target, size_t* StringForwardsIterator);
            int startsWith(const String* const str, const String* const target, bool* doesStartWith);
            int endsWith(const String* const str, const String* const target, bool* doesEndWith);

            int frontForwardsIterator(const String* const str, size_t* StringForwardsIterator);

            int isAllLower(const String* const str, bool* result);
            int isAllUpper(const String* const str, bool* result);
            int isWhiteSpace(const String* const str, bool* result);
            int isPrintable(const String* const str, bool* result);
            int isAscii(const String* const str, bool* result);
            int isDecimal(const String* const str, bool* result);
            int isInt(const String* const str, bool* result);
            int isFloat(const String* const str, bool* result);
            int isAlpha(const String* const str, bool* result);
            int isAlphaNumeric(const String* const str, bool* result);

            int valueEqualityStrStr(const String* const str1, const String* const str2);
            int valueEqualityStrChr(const String* const str, const Character* const chr);

            int strip(const String* const str);
            int lStrip(const String* const str);
            int rStrip(const String* const str);
            int replace(const String* const str, const String* const oldValue, const String* const newValue);
            int toUpper(const String* const str);
            int toLower(const String* const str);
            
            int substrIndex(const String* const originalStr, String** newStr, const size_t& startIndex, const size_t& endIndex);
            int substrForwardsIterator(const String* const originalStr, String** newStr, const StringForwardsIterator& startIt, const StringForwardsIterator& endIt);

            //================ Character functions ================

            int isAllLowerChr(const Character* const str, bool* result);
            int isAllUpperChr(const Character* const str, bool* result);
            int isWhiteSpaceChr(const Character* const str, bool* result);
            int isPrintableChr(const Character* const str, bool* result);
            int isAsciiChr(const Character* const str, bool* result);
            int isDecimalChr(const Character* const str, bool* result);
            int isIntChr(const Character* const str, bool* result);
            int isFloatChr(const Character* const str, bool* result);
            int isAlphaChr(const Character* const str, bool* result);
            int isAlphaNumericChr(const Character* const str, bool* result);

            int valueEqualityChrChr(const Character* const chr1, const Character* const chr2);
            int valueEqualityChrStr(const Character* const chr, const String* const str);

            int replaceChr(const Character* const str, const Character* const oldValue, const Character* const newValue);
            int toUpperChr(const Character* const str);
            int toLowerChr(const Character* const str);

            //================ String iterator ================

            int getCurrent(const StringForwardsIterator* const it, Character* dst);
            int next(StringForwardsIterator* it);
            int isFinished(const StringForwardsIterator* const it, bool* finished);

            //================ Constructors ================

            int createStringRefFromCStr(StringRef* const strRef, const char* cStr, const size_t& numBytes, const size_t& numCharacters);
            int copyStringRef(const StringRef originalStrRef, StringRef* const newStrRef);
            int destroyStringRef(const StringRef strRef);

            namespace internal
            {
                int createRawString(RawString** newStr, const char* data, const size_t& numBytes);
                int createRawStringNoCopy(RawString** newStr, const size_t& numBytes);
                int destroyRawString(RawString* const str);

                int createStringWithRawStr(String* const* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr);
                int createStringNoRawStr(String* const* newStr);
                int initialiseExistingString(String* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr);
                int destroyString(String* const str);

                int normaliseString(String* const str);
            }
        }
    }
}

#endif //STRING_HEADER_GUARD