#ifndef STRING_HEADER_GUARD
#define STRING_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "../runtime_lib/allocator.hpp"
#include "../../vendor/utf8proc/utf8proc.h"

const uint32_t MDPL_STDLIB_STRING_FLAGS_isLower        = 1;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isUpper        = 2;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isWhiteSpace   = 4;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isPrintable    = 8;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isAscii        = 16;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isValidDecimal = 32;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isValidInt     = 64;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isValidFloat   = 128;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isAlpha        = 256;
const uint32_t MDPL_STDLIB_STRING_FLAGS_isAlphaNumeric = 512;

//RawString
typedef struct
{
    size_t refCount;
    const size_t numBytes;
    const char str[1];
}  MDPL_STDLIB_STRING_RawString;
//String
typedef struct
{
    size_t refCount;
    size_t numCharacters;
    size_t startByte;
    size_t endByte; //up to but not including
    uint32_t flagsData;
    uint32_t flagsSet;
    MDPL_STDLIB_STRING_RawString* const rawStr;
    MDPL_STDLIB_STRING_RawString* normalisedStr;
} MDPL_STDLIB_STRING_String;
//StringRef
typedef struct
{
    MDPL_STDLIB_STRING_String* const s;
} MDPL_STDLIB_STRING_StringRef;
//Character
typedef struct
{
    uint32_t codepoint;
} MDPL_STDLIB_STRING_Character;
//StringIterator
typedef struct
{
    const MDPL_STDLIB_STRING_String* const str;
    size_t byteIndex;
    size_t characterIndex;
    int32_t step;
} MDPL_STDLIB_STRING_StringIterator;

const size_t npos = static_cast<size_t>(-1);

//================ String functions ================

int MDPL_STDLIB_STRING_count(const MDPL_STDLIB_STRING_StringRef str, size_t* ocurances);
int MDPL_STDLIB_STRING_findIndex(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef target, size_t* characterIndex);
int MDPL_STDLIB_STRING_rFindIndex(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef target, size_t* characterIndex);
int MDPL_STDLIB_STRING_findForwardsIterator(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef target, size_t* StringIterator);
int MDPL_STDLIB_STRING_rFindForwardsIterator(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef target, size_t* StringIterator);
int MDPL_STDLIB_STRING_startsWith(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef prefix, bool* result);
int MDPL_STDLIB_STRING_endsWith(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef suffix, bool* result);

int MDPL_STDLIB_STRING_isLower(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isUpper(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isWhiteSpace(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isPrintable(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isAscii(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isValidDecimal(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isValidInt(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isValidFloat(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isAlpha(const MDPL_STDLIB_STRING_StringRef str, bool* result);
int MDPL_STDLIB_STRING_isAlphaNumeric(const MDPL_STDLIB_STRING_StringRef str, bool* result);

int MDPL_STDLIB_STRING_valueEqualityStrStr(const MDPL_STDLIB_STRING_String* const str1, const MDPL_STDLIB_STRING_String* const str2, bool* result);

int MDPL_STDLIB_STRING_strip(const MDPL_STDLIB_STRING_StringRef str);
int MDPL_STDLIB_STRING_lStrip(const MDPL_STDLIB_STRING_StringRef str);
int MDPL_STDLIB_STRING_rStrip(const MDPL_STDLIB_STRING_StringRef str);
int MDPL_STDLIB_STRING_replace(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef oldValue, const MDPL_STDLIB_STRING_StringRef newValue);
int MDPL_STDLIB_STRING_toUpper(const MDPL_STDLIB_STRING_StringRef str);
int MDPL_STDLIB_STRING_toLower(const MDPL_STDLIB_STRING_StringRef str);

int MDPL_STDLIB_STRING_substrIndex(const MDPL_STDLIB_STRING_StringRef originalStr, MDPL_STDLIB_STRING_StringRef* const newStr, const size_t& startIndex, const size_t& endIndex);
int MDPL_STDLIB_STRING_substrIterator(const MDPL_STDLIB_STRING_StringRef originalStr, MDPL_STDLIB_STRING_StringRef* const newStr, const MDPL_STDLIB_STRING_StringIterator& startIt, const MDPL_STDLIB_STRING_StringIterator& endIt);

//================ Character functions ================

int MDPL_STDLIB_STRING_isLowerChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isUpperChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isWhiteSpaceChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isPrintableChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isAsciiChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isDecimalChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isIntChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isFloatChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isAlphaChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isAlphaNumericChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isNewLineChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);
int MDPL_STDLIB_STRING_isNullChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result);

int MDPL_STDLIB_STRING_valueEqualityChrChr(const MDPL_STDLIB_STRING_Character* const chr1, const MDPL_STDLIB_STRING_Character* const chr2, bool* result);
int MDPL_STDLIB_STRING_valueEqualityChrUnicode(const MDPL_STDLIB_STRING_Character* const chr1, const uint32_t codepoint, bool* result);

int MDPL_STDLIB_STRING_toUpperChr(const MDPL_STDLIB_STRING_Character* const originalChr, MDPL_STDLIB_STRING_Character* const newChr);
int MDPL_STDLIB_STRING_toLowerChr(const MDPL_STDLIB_STRING_Character* const originalChr, MDPL_STDLIB_STRING_Character* const newChr);

//================ String iterator ================

int MDPL_STDLIB_STRING_frontForwardsIterator(const MDPL_STDLIB_STRING_StringRef str, MDPL_STDLIB_STRING_StringIterator* const iterator);
int MDPL_STDLIB_STRING_backReverseIterator(const MDPL_STDLIB_STRING_StringRef str, MDPL_STDLIB_STRING_StringIterator* const iterator);

int MDPL_STDLIB_STRING_getCurrent(const MDPL_STDLIB_STRING_StringIterator* const it, MDPL_STDLIB_STRING_Character* dst);
int MDPL_STDLIB_STRING_next(MDPL_STDLIB_STRING_StringIterator* it);
int MDPL_STDLIB_STRING_isFinished(const MDPL_STDLIB_STRING_StringIterator* const it, bool* finished);

//================ Constructors ================

int MDPL_STDLIB_STRING_createStringRefFromCStr(MDPL_STDLIB_STRING_StringRef* const strRef, const char* cStr, const size_t& numBytes, const size_t& numCharacters);
int MDPL_STDLIB_STRING_copyStringRef(const MDPL_STDLIB_STRING_StringRef originalStrRef, MDPL_STDLIB_STRING_StringRef* const newStrRef);
int MDPL_STDLIB_STRING_destroyStringRef(const MDPL_STDLIB_STRING_StringRef strRef);

int MDPL_STDLIB_STRING_INTERNAL_createRawString(MDPL_STDLIB_STRING_RawString* const* newStr, const char* data, const size_t& numBytes);
int MDPL_STDLIB_STRING_INTERNAL_createRawStringNoCopy(MDPL_STDLIB_STRING_RawString* const* newStr, const size_t& numBytes);
int MDPL_STDLIB_STRING_INTERNAL_destroyRawString(MDPL_STDLIB_STRING_RawString* const str);

int MDPL_STDLIB_STRING_INTERNAL_createStringWithRawStr(MDPL_STDLIB_STRING_String* const* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, MDPL_STDLIB_STRING_RawString* const rawStr);
int MDPL_STDLIB_STRING_INTERNAL_createStringNoRawStr(MDPL_STDLIB_STRING_String* const* newStr);
int MDPL_STDLIB_STRING_INTERNAL_initialiseExistingString(MDPL_STDLIB_STRING_String* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, MDPL_STDLIB_STRING_RawString* const rawStr);
int MDPL_STDLIB_STRING_INTERNAL_copyString(const MDPL_STDLIB_STRING_String* const originalStr, MDPL_STDLIB_STRING_String* const* newStr);
int MDPL_STDLIB_STRING_INTERNAL_destroyString(MDPL_STDLIB_STRING_String* const str);

int MDPL_STDLIB_STRING_INTERNAL_normaliseString(MDPL_STDLIB_STRING_String* const str);

int MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(const MDPL_STDLIB_STRING_StringRef str, const utf8proc_uint8_t** ptr);
int MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(const MDPL_STDLIB_STRING_StringRef str, const utf8proc_uint8_t** ptr, utf8proc_int32_t* character);

#endif //STRING_HEADER_GUARD