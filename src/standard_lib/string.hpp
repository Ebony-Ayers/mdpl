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
            
            namespace StringFlags
            {
                const uint32_t isLower        = 1;
                const uint32_t isUpper        = 2;
                const uint32_t isWhiteSpace   = 4;
                const uint32_t isPrintable    = 8;
                const uint32_t isAscii        = 16;
                const uint32_t isValidDecimal = 32;
                const uint32_t isValidInt     = 64;
                const uint32_t isValidFloat   = 128;
                const uint32_t isAlpha        = 256;
                const uint32_t isAlphaNumeric = 512;
            };
            
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
                uint32_t codepoint;
            };
            struct StringIterator
            {
                const String* const str;
                size_t byteIndex;
                size_t characterIndex;
                int32_t step;
            };

            const size_t npos = static_cast<size_t>(-1);

            //================ String functions ================
            
            int count(const StringRef str, size_t* ocurances);
            int findIndex(const StringRef str, const String* const target, size_t* characterIndex);
            int rFindIndex(const StringRef str, const String* const target, size_t* characterIndex);
            int findForwardsIterator(const StringRef str, const String* const target, size_t* StringIterator);
            int rFindForwardsIterator(const StringRef str, const String* const target, size_t* StringIterator);
            int startsWith(const StringRef str, const StringRef prefix, bool* result);
            int endsWith(const StringRef str, const StringRef suffix, bool* result);

            int isLower(const StringRef str, bool* result);
            int isUpper(const StringRef str, bool* result);
            int isWhiteSpace(const StringRef str, bool* result);
            int isPrintable(const StringRef str, bool* result);
            int isAscii(const StringRef str, bool* result);
            int isValidDecimal(const StringRef str, bool* result);
            int isValidInt(const StringRef str, bool* result);
            int isValidFloat(const StringRef str, bool* result);
            int isAlpha(const StringRef str, bool* result);
            int isAlphaNumeric(const StringRef str, bool* result);

            int valueEqualityStrStr(const String* const str1, const String* const str2, bool* result);

            int strip(const String* const str);
            int lStrip(const String* const str);
            int rStrip(const String* const str);
            int replace(const String* const str, const String* const oldValue, const String* const newValue);
            int toUpper(const String* const str);
            int toLower(const String* const str);
            
            int substrIndex(const StringRef originalStr, StringRef* const newStr, const size_t& startIndex, const size_t& endIndex);
            int substrIterator(const StringRef originalStr, StringRef* const newStr, const StringIterator& startIt, const StringIterator& endIt);

            //================ Character functions ================

            int isLowerChr(const Character* const chr, bool* result);
            int isUpperChr(const Character* const chr, bool* result);
            int isWhiteSpaceChr(const Character* const chr, bool* result);
            int isPrintableChr(const Character* const chr, bool* result);
            int isAsciiChr(const Character* const chr, bool* result);
            int isDecimalChr(const Character* const chr, bool* result);
            int isIntChr(const Character* const chr, bool* result);
            int isFloatChr(const Character* const chr, bool* result);
            int isAlphaChr(const Character* const chr, bool* result);
            int isAlphaNumericChr(const Character* const chr, bool* result);
            int isNewLineChr(const Character* const chr, bool* result);
            int isNullChr(const Character* const chr, bool* result);

            int valueEqualityChrChr(const Character* const chr1, const Character* const chr2, bool* result);
            int valueEqualityChrUnicode(const Character* const chr1, const uint32_t codepoint, bool* result);

            int toUpperChr(const Character* const originalChr, Character* const newChr);
            int toLowerChr(const Character* const originalChr, Character* const newChr);

            //================ String iterator ================

            int frontForwardsIterator(const StringRef str, StringIterator* const iterator);
            int backReverseIterator(const StringRef str, StringIterator* const iterator);

            int getCurrent(const StringIterator* const it, Character* dst);
            int next(StringIterator* it);
            int isFinished(const StringIterator* const it, bool* finished);

            //================ Constructors ================

            int createStringRefFromCStr(StringRef* const strRef, const char* cStr, const size_t& numBytes, const size_t& numCharacters);
            int copyStringRef(const StringRef originalStrRef, StringRef* const newStrRef);
            int destroyStringRef(const StringRef strRef);

            namespace internal
            {
                int createRawString(RawString* const* newStr, const char* data, const size_t& numBytes);
                int createRawStringNoCopy(RawString* const* newStr, const size_t& numBytes);
                int destroyRawString(RawString* const str);

                int createStringWithRawStr(String* const* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr);
                int createStringNoRawStr(String* const* newStr);
                int initialiseExistingString(String* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr);
                int copyString(const String* const originalStr, String* const* newStr);
                int destroyString(String* const str);

                int normaliseString(String* const str);

                int createInternalIterator(const StringRef str, const utf8proc_uint8_t** ptr);
                int incrementInternalIterator(const StringRef str, const utf8proc_uint8_t** ptr, utf8proc_int32_t* character);
            }
        }
    }
}

#endif //STRING_HEADER_GUARD