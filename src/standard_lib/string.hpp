#ifndef STRING_HEADER_GUARD
#define STRING_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "../runtime_lib/allocator.hpp"

namespace mdpl
{
    namespace standardLibrary
    {
        namespace String
        {
            struct RawString
            {
                size_t refCount;
                const size_t numBytes;
                char str[1];
            };
            struct String
            {
                size_t numCharacters;
                size_t startByte;
                size_t endByte;
                const RawString* const str;
            };
            struct Character
            {
                size_t startByte;
                size_t endByte;
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

            int isAllLower(const String* const str, bool* isLower);
            int isAllUpper(const String* const str, bool* isLower);
            int isWhiteSpace(const String* const str, bool* isLower);
            int isPrintable(const String* const str, bool* isLower);
            int isAscii(const String* const str, bool* isLower);
            int isDecimal(const String* const str, bool* isLower);
            int isInt(const String* const str, bool* isLower);
            int isFloat(const String* const str, bool* isLower);
            int isAlpha(const String* const str, bool* isLower);
            int isAlphaNumeric(const String* const str, bool* isLower);

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

            int isAllLowerChr(const Character* const str, bool* isLower);
            int isAllUpperChr(const Character* const str, bool* isLower);
            int isWhiteSpaceChr(const Character* const str, bool* isLower);
            int isPrintableChr(const Character* const str, bool* isLower);
            int isAsciiChr(const Character* const str, bool* isLower);
            int isDecimalChr(const Character* const str, bool* isLower);
            int isIntChr(const Character* const str, bool* isLower);
            int isFloatChr(const Character* const str, bool* isLower);
            int isAlphaChr(const Character* const str, bool* isLower);
            int isAlphaNumericChr(const Character* const str, bool* isLower);

            int valueEqualityChrChr(const Character* const chr1, const Character* const chr2);
            int valueEqualityChrStr(const Character* const chr, const String* const str);

            int replaceChr(const Character* const str, const Character* const oldValue, const Character* const newValue);
            int toUpperChr(const Character* const str);
            int toLowerChr(const Character* const str);

            //================ String iterator ================

            int getCurrent(const StringForwardsIterator* const it, Character* dst);
            int next(StringForwardsIterator* it);
            int isFinished(const StringForwardsIterator* const it, bool* finished);

            //================ Internal functions ================

            int createString(String** newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, const RawString* const str);
            int copyString(const String* const originalStr, String** newStr);
            int destroyString(const String* const str);

            int createRawString(RawString** newStr, const size_t& numBytes);
            int destroyRawString(const RawString* const str);
        }
    }
}

#endif //STRING_HEADER_GUARD