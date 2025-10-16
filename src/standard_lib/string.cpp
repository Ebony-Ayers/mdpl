#include "string.hpp"

//================ String functions ================

int MDPL_STDLIB_STRING_startsWith(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef prefix, bool* result)
{
    //if the prefix is longer than the string then by definition it cannot be a prefix
    if(prefix.s->numCharacters > str.s->numCharacters)
    {
        *result = false;
        return 0;
    }

    //make sure the strings are normalised for comparison
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_normaliseString(str.s));
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_normaliseString(prefix.s));
    
    //if the prefix has more bytes then the str after normalisation they cannot be the same
    if(prefix.s->normalisedStr->numBytes > str.s->normalisedStr->numBytes)
    {
        *result = false;
        return 0;
    }

    //compare the normalised strings byte by byte. As the strings are normalised this is a valid way to compare them.
    //this method is optimal for both ascii and non-ascii strings
    const size_t numBytes = prefix.s->normalisedStr->numBytes;
    for(size_t i = 0; i < numBytes; i++)
    {
        if(str.s->normalisedStr->str[i] != prefix.s->normalisedStr->str[i])
        {
            *result = false;
            return 0;
        }
    }
    *result = true;
    return 0;
}
int MDPL_STDLIB_STRING_endsWith(const MDPL_STDLIB_STRING_StringRef str, const MDPL_STDLIB_STRING_StringRef suffix, bool* result)
{
    //if the prefix is longer than the string then by definition it cannot be a prefix
    if(suffix.s->numCharacters > str.s->numCharacters)
    {
        *result = false;
        return 0;
    }

    //make sure the strings are normalised for comparison
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_normaliseString(str.s));
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_normaliseString(suffix.s));
    
    //if the prefix has more bytes then the str after normalisation they cannot be the same
    if(suffix.s->normalisedStr->numBytes > str.s->normalisedStr->numBytes)
    {
        *result = false;
        return 0;
    }

    //compare the normalised strings byte by byte. As the strings are normalised this is a valid way to compare them.
    //this method is optimal for both ascii and non-ascii strings
    const size_t numBytesStr = str.s->normalisedStr->numBytes;
    const size_t numBytesSuffix = suffix.s->normalisedStr->numBytes;
    //start iStr numBytesSuffix back from the end and compare forwards
    for(size_t iStr = numBytesStr - numBytesSuffix, iSuffix = 0; (iStr < numBytesStr) && (iSuffix < numBytesSuffix); iStr++, iSuffix++)
    {
        if(str.s->normalisedStr->str[iStr] != suffix.s->normalisedStr->str[iSuffix])
        {
            *result = false;
            return 0;
        }
    }
    *result = true;
    return 0;
}

int MDPL_STDLIB_STRING_isLower(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isLower) [[likely]]
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isLower ? true : false;
        return 0;
    }

    *result = true;
    const utf8proc_uint8_t* ptr;
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(str, &ptr));
    for (size_t i = 0; i < str.s->numCharacters; i++)
    {
        utf8proc_int32_t character;
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(str, &ptr, &character));
        const utf8proc_property_t *p = utf8proc_get_property(character);
        if((p->lowercase_seqindex != UINT16_MAX) && (p->uppercase_seqindex == UINT16_MAX))
        {
            *result = false;
            str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isLower;
            str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isLower;
            return 0;
        }
    }
    str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isLower;
    str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isLower;
    return 0;
}
int MDPL_STDLIB_STRING_isUpper(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isUpper) [[likely]]
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isUpper ? true : false;
        return 0;
    }

    *result = true;
    const utf8proc_uint8_t* ptr;
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(str, &ptr));
    for (size_t i = 0; i < str.s->numCharacters; i++)
    {
        utf8proc_int32_t character;
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(str, &ptr, &character));
        const utf8proc_property_t *p = utf8proc_get_property(character);
        if((p->lowercase_seqindex == UINT16_MAX) && (p->uppercase_seqindex != UINT16_MAX))
        {
            *result = false;
            str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isUpper;
            str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isUpper;
            return 0;
        }
    }
    str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isUpper;
    str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isUpper;
    return 0;
}
int MDPL_STDLIB_STRING_isWhiteSpace(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isWhiteSpace) [[likely]]
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isWhiteSpace ? true : false;
        return 0;
    }

    *result = true;
    const utf8proc_uint8_t* ptr;
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(str, &ptr));
    for (size_t i = 0; i < str.s->numCharacters; i++)
    {
        utf8proc_int32_t character;
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(str, &ptr, &character));
        const utf8proc_property_t *p = utf8proc_get_property(character);
        if(!((p->bidi_class == UTF8PROC_BIDI_CLASS_B) || (p->bidi_class == UTF8PROC_BIDI_CLASS_S) || (p->bidi_class == UTF8PROC_BIDI_CLASS_WS)))
        {
            *result = false;
            str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isWhiteSpace;
            str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isWhiteSpace;
            return 0;
        }
    }
    str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isWhiteSpace;
    str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isWhiteSpace;
    return 0;
}
int MDPL_STDLIB_STRING_isPrintable(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isPrintable) [[likely]]
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isPrintable ? true : false;
        return 0;
    }

    *result = true;
    const utf8proc_uint8_t* ptr;
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(str, &ptr));
    for (size_t i = 0; i < str.s->numCharacters; i++)
    {
        utf8proc_int32_t character;
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(str, &ptr, &character));
        //a char width of 0 indicates the character is non-printable
        if(utf8proc_charwidth(character) == 0)
        {
            *result = false;
            str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isPrintable;
            str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isPrintable;
            return 0;
        }
    }
    str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isPrintable;
    str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isPrintable;
    return 0;
}
int MDPL_STDLIB_STRING_isAscii(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isAscii) [[likely]]
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isAscii ? true : false;
        return 0;
    }
    else
    {
        for(size_t i = str.s->startByte; i < str.s->endByte; i++)
        {
            if(str.s->rawStr->str[i] & 0b10000000)
            {
                *result = false;
                str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isAscii;
                str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isAscii;
                return 0;
            }
        }
        *result = true;
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isAscii;
        str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isAscii;
        return 0;
    }
}
int MDPL_STDLIB_STRING_isValidDecimal(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    //check if a previous call to this function has been done
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isValidDecimal)
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isValidDecimal ? true : false;
        return 0;
    }
    //if the stirng is not ascii then by definition it cannot be a decimal
    bool ascii;
    MDPL_RETERR(MDPL_STDLIB_STRING_isAscii(str, &ascii));
    if(!ascii)
    {
        *result = false;
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidDecimal;
        str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isValidDecimal;
        return 0;
    }
    else
    {
        for(size_t i = str.s->startByte; i < str.s->endByte; i++)
        {
            const char c = str.s->rawStr->str[i];
            if(! ((c >= '0') && (c <= '9')) )
            {
                *result = false;
                str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidDecimal;
                str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isValidDecimal;
                return 0;
            }
        }
        *result = true;
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidDecimal;
        str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isValidDecimal;
        return 0;
    }
}
int MDPL_STDLIB_STRING_isValidInt(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    //check if a previous call to this function has been done
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isValidInt)
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isValidInt ? true : false;
        return 0;
    }
    //if the stirng is not ascii then by definition it cannot be an int
    bool ascii;
    MDPL_RETERR(MDPL_STDLIB_STRING_isAscii(str, &ascii));
    if(!ascii)
    {
        *result = false;
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidInt;
        str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isValidInt;
        return 0;
    }
    else
    {
        //if the first character is a - then we can skip checking it
        const size_t firstMDPL_STDLIB_STRING_CharacterHythenOffset = str.s->rawStr->str[str.s->startByte] == '-' ? 1 : 0;
        for(size_t i = str.s->startByte + firstMDPL_STDLIB_STRING_CharacterHythenOffset; i < str.s->endByte; i++)
        {
            const char c = str.s->rawStr->str[i];
            if(! ((c >= '0') && (c <= '9')) )
            {
                *result = false;
                str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidInt;
                str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isValidInt;
                return 0;
            }
        }
        *result = true;
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidInt;
        str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isValidInt;
        return 0;
    }
}
int MDPL_STDLIB_STRING_isValidFloat(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    //check if a previous call to this function has been done
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isValidFloat)
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isValidFloat ? true : false;
        return 0;
    }
    //if the stirng is not ascii then by definition it cannot be an int
    bool ascii;
    MDPL_RETERR(MDPL_STDLIB_STRING_isAscii(str, &ascii));
    if(!ascii)
    {
        *result = false;
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidFloat;
        str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isValidFloat;
        return 0;
    }
    else
    {
        //if the first character is a - then we can skip checking it
        const size_t firstMDPL_STDLIB_STRING_CharacterHythenOffset = str.s->rawStr->str[str.s->startByte] == '-' ? 1 : 0;
        bool alreadyFoundDecimalPoint = false;
        for(size_t i = str.s->startByte + firstMDPL_STDLIB_STRING_CharacterHythenOffset; i < str.s->endByte; i++)
        {
            const char c = str.s->rawStr->str[i];
            if(! ((c >= '0') && (c <= '9')) )
            {
                if(c == '.')
                {
                    //a float can only have a single decimal point so if we have already found it, it is not valid
                    if(alreadyFoundDecimalPoint)
                    {
                        *result = false;
                        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidFloat;
                        str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isValidFloat;
                        return 0;
                    }
                    else
                    {
                        alreadyFoundDecimalPoint = true;
                    }
                }
                else
                {
                    *result = false;
                    str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidFloat;
                    str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isValidFloat;
                    return 0;
                }
            }
        }
        *result = true;
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isValidFloat;
        str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isValidFloat;
        return 0;
    }
}
int MDPL_STDLIB_STRING_isAlpha(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isAlpha) [[likely]]
    {
        *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isAlpha ? true : false;
        return 0;
    }

    *result = true;
    const utf8proc_uint8_t* ptr;
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(str, &ptr));
    for (size_t i = 0; i < str.s->numCharacters; i++)
    {
        utf8proc_int32_t character;
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(str, &ptr, &character));
        const utf8proc_property_t *p = utf8proc_get_property(character);
        if((p->category == 0) || (p->category > 5))
        {
            *result = false;
            str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isAlpha;
            str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isAlpha;
            return 0;
        }
    }
    str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isAlpha;
    str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isAlpha;
    return 0;
}
int MDPL_STDLIB_STRING_isAlphaNumeric(const MDPL_STDLIB_STRING_StringRef str, bool* result)
{
    {
        if(str.s->flagsSet & MDPL_STDLIB_STRING_FLAGS_isAlphaNumeric) [[likely]]
        {
            *result = str.s->flagsData & MDPL_STDLIB_STRING_FLAGS_isAlphaNumeric ? true : false;
            return 0;
        }

        *result = true;
        const utf8proc_uint8_t* ptr;
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(str, &ptr));
        for (size_t i = 0; i < str.s->numCharacters; i++)
        {
            utf8proc_int32_t character;
            MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(str, &ptr, &character));
            const utf8proc_property_t *p = utf8proc_get_property(character);
            if( !(((p->category > 0) && (p->category < 6)) || ((p->category > 8) && (p->category < 12))) )
            {
                *result = false;
                str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isAlphaNumeric;
                str.s->flagsData &= ~MDPL_STDLIB_STRING_FLAGS_isAlphaNumeric;
                return 0;
            }
        }
        str.s->flagsSet |= MDPL_STDLIB_STRING_FLAGS_isAlphaNumeric;
        str.s->flagsData |= MDPL_STDLIB_STRING_FLAGS_isAlphaNumeric;
        return 0;
    }
}

int MDPL_STDLIB_STRING_substrIndex(const MDPL_STDLIB_STRING_StringRef originalStr, MDPL_STDLIB_STRING_StringRef* const newStr, const size_t& startIndex, const size_t& endIndex)
{
    if(startIndex > endIndex)
    {
        printf("String error: substrIndex() start index greater than end index.\n");
        return 1;
    }
    if(startIndex == endIndex)
    {
        printf("String error: substrIndex() start index same as end index.\n");
        return 1;
    }

    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_copyString(originalStr.s, &newStr->s));
    newStr->s->numCharacters = endIndex - startIndex;
    
    bool result;
    MDPL_RETERR(MDPL_STDLIB_STRING_isAscii(originalStr, &result));
    if(result)
    {
        newStr->s->startByte = startIndex;
        newStr->s->endByte = endIndex;
    }
    else
    {
        const utf8proc_uint8_t* startIt;
        const utf8proc_uint8_t* movingvIt;
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(originalStr, &startIt));
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(originalStr, &movingvIt));
        for(size_t i = 0; i < startIndex; i++) { utf8proc_int32_t c; MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(originalStr, &movingvIt, &c)); }
        newStr->s->startByte = (size_t)(movingvIt - startIt);
        for(size_t i = startIndex; i < endIndex; i++) { utf8proc_int32_t c; MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(originalStr, &movingvIt, &c)); }
        newStr->s->endByte = (size_t)(movingvIt - startIt);
    }
    const uint32_t carryOverFlags = originalStr.s->flagsSet & originalStr.s->flagsData;
    newStr->s->flagsData = carryOverFlags;
    newStr->s->flagsSet = carryOverFlags; 
    newStr->s->normalisedStr = nullptr;
    return 0;
}
int MDPL_STDLIB_STRING_substrIterator(const MDPL_STDLIB_STRING_StringRef originalStr, MDPL_STDLIB_STRING_StringRef* const newStr, const MDPL_STDLIB_STRING_StringIterator& startIt, const MDPL_STDLIB_STRING_StringIterator& endIt)
{
    if(((startIt.step > 1) || (startIt.step < -1)) && ((endIt.step > 1) || (endIt.step < -1)))
    {
        printf("String error: substrIiterator() steps other than 1 not yet implemented.\n");
        return 1;
    }
    if((startIt.characterIndex > endIt.characterIndex) && ((startIt.step < 0) || (endIt.step < 0)))
    {
        printf("String error: substrIiterator() reverse substrings not yet implemented.\n");
        return 1;
    }
    if(startIt.characterIndex > endIt.characterIndex)
    {
        printf("String error: substrIiterator() start iterator after end iterator.\n");
        return 1;
    }
    if(startIt.characterIndex == endIt.characterIndex)
    {
        printf("String error: substrIiterator() start iterator same as end iterator.\n");
        return 1;
    }
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_copyString(originalStr.s, &newStr->s));
    newStr->s->numCharacters = endIt.characterIndex - startIt.characterIndex;
    newStr->s->startByte = startIt.byteIndex;
    newStr->s->endByte = endIt.byteIndex;
    const uint32_t carryOverFlags = originalStr.s->flagsSet & originalStr.s->flagsData;
    newStr->s->flagsData = carryOverFlags;
    newStr->s->flagsSet = carryOverFlags; 
    newStr->s->normalisedStr = nullptr;
    return 0;
}

//================ Character functions ================

int MDPL_STDLIB_STRING_isLowerChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    const utf8proc_property_t *p = utf8proc_get_property((utf8proc_int32_t)(chr->codepoint));
    *result = (p->lowercase_seqindex == UINT16_MAX) && (p->uppercase_seqindex != UINT16_MAX);
    return 0;
}
int MDPL_STDLIB_STRING_isUpperChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    const utf8proc_property_t *p = utf8proc_get_property((utf8proc_int32_t)(chr->codepoint));
    *result = (p->lowercase_seqindex != UINT16_MAX) && (p->uppercase_seqindex == UINT16_MAX);
    return 0;
}
int MDPL_STDLIB_STRING_isWhiteSpaceChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    const utf8proc_property_t *p = utf8proc_get_property((utf8proc_int32_t)(chr->codepoint));
    *result = (p->bidi_class == UTF8PROC_BIDI_CLASS_B) || (p->bidi_class == UTF8PROC_BIDI_CLASS_S) || (p->bidi_class == UTF8PROC_BIDI_CLASS_WS);
    return 0;
}
int MDPL_STDLIB_STRING_isPrintableChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    *result = utf8proc_charwidth((utf8proc_int32_t)(chr->codepoint)) != 0;
    return 0;
}
int MDPL_STDLIB_STRING_isAsciiChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    *result = chr->codepoint < 128;
    return 0;
}
int MDPL_STDLIB_STRING_isDecimalChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    *result = ((chr->codepoint >= '0') && (chr->codepoint <= '9'));
    return 0;
}
int MDPL_STDLIB_STRING_isIntChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    *result = ((chr->codepoint >= '0') && (chr->codepoint <= '9')) || (chr->codepoint == '-');
    return 0;
}
int MDPL_STDLIB_STRING_isFloatChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    *result = ((chr->codepoint >= '0') && (chr->codepoint <= '9')) || (chr->codepoint == '-') || (chr->codepoint == '.');
    return 0;
}
int MDPL_STDLIB_STRING_isAlphaChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    const utf8proc_property_t *p = utf8proc_get_property((utf8proc_int32_t)(chr->codepoint));
    *result = (p->category > 0) && (p->category < 6);
    return 0;
}
int MDPL_STDLIB_STRING_isAlphaNumericChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    const utf8proc_property_t *p = utf8proc_get_property((utf8proc_int32_t)(chr->codepoint));
    *result = ((p->category > 0) && (p->category < 6)) || ((p->category > 8) && (p->category < 12));
    return 0;
}
int MDPL_STDLIB_STRING_isNewLineChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    *result = (chr->codepoint == '\n') || (chr->codepoint == '\r');
    return 0;
}
int MDPL_STDLIB_STRING_isNullChr(const MDPL_STDLIB_STRING_Character* const chr, bool* result)
{
    *result = chr->codepoint == '\0';
    return 0;
}

int MDPL_STDLIB_STRING_valueEqualityChrChr(const MDPL_STDLIB_STRING_Character* const chr1, const MDPL_STDLIB_STRING_Character* const chr2, bool* result)
{
    *result = chr1->codepoint == chr2->codepoint;
    return 0;
}
int MDPL_STDLIB_STRING_valueEqualityChrUnicode(const MDPL_STDLIB_STRING_Character* const chr1, const uint32_t codepoint, bool* result)
{
    *result = chr1->codepoint == codepoint;
    return 0;
}

int MDPL_STDLIB_STRING_toLowerChr(const MDPL_STDLIB_STRING_Character* const originalChr, MDPL_STDLIB_STRING_Character* const newChr)
{
    *newChr = { (uint32_t)(utf8proc_tolower((utf8proc_int32_t)(originalChr->codepoint))) };
    return 0;
}
int MDPL_STDLIB_STRING_toUpperChr(const MDPL_STDLIB_STRING_Character* const originalChr, MDPL_STDLIB_STRING_Character* const newChr)
{
    *newChr = { (uint32_t)(utf8proc_toupper((utf8proc_int32_t)(originalChr->codepoint))) };
    return 0;
}

//================ String iterator ================

int MDPL_STDLIB_STRING_frontForwardsIterator(const MDPL_STDLIB_STRING_StringRef str, MDPL_STDLIB_STRING_StringIterator* const iterator)
{
    *((MDPL_STDLIB_STRING_String**)&iterator->str) = str.s;
    iterator->byteIndex = 0;
    iterator->characterIndex = 0;
    iterator->step = 1;
    return 0;
}
int MDPL_STDLIB_STRING_backReverseIterator(const MDPL_STDLIB_STRING_StringRef str, MDPL_STDLIB_STRING_StringIterator* const iterator)
{
    *((MDPL_STDLIB_STRING_String**)&iterator->str) = str.s;
    iterator->byteIndex = str.s->rawStr->numBytes - 1;
    for(size_t _ = 0; _ < 4; _++)
    {
        if((iterator->str->rawStr->str[iterator->byteIndex] & 0b11000000) != 0b10000000)
        {
            break;
        }
        iterator->byteIndex--;
    }
    iterator->characterIndex = str.s->numCharacters - 1;
    iterator->step = -1;
    return 0;
}

int MDPL_STDLIB_STRING_getCurrent(const MDPL_STDLIB_STRING_StringIterator* const it, MDPL_STDLIB_STRING_Character* dst)
{
    utf8proc_ssize_t retcode = utf8proc_iterate((const utf8proc_uint8_t*)(it->str->rawStr->str + it->byteIndex), -1, (utf8proc_int32_t*)(&(dst->codepoint)));
    if(retcode < 0)
    {
        printf("String error: \"%s\" occured during string iterator getCurrrent.\n", utf8proc_errmsg(retcode));
        return 1;
    }
    return 0;
}
int MDPL_STDLIB_STRING_next(MDPL_STDLIB_STRING_StringIterator* it)
{
    utf8proc_int32_t chracter;
    if(it->step == 0) [[unlikely]]
    {
        printf("String error: iterator step size set to 0.\n");
        return 1;
    }
    else if(it->step > 0)
    {
        for(int32_t i = 0; i < it->step; i++)
        {
            utf8proc_ssize_t retcode = utf8proc_iterate((const utf8proc_uint8_t*)(it->str->rawStr->str + it->byteIndex), -1, &chracter);
            if(retcode < 0)
            {
                printf("String error: \"%s\" occured during string iterator next.\n", utf8proc_errmsg(retcode));
                return 1;
            }
            it->byteIndex += (size_t)(retcode);
        }
        it->characterIndex += (size_t)(it->step);
        return 0;
    }
    else
    {
        for(int32_t i = 0; i < -(it->step); i++)
        {
            //as we will be pointing to the first byte decrement by one so that we either go to the previous ascii and skip or we go to the previous multibyte and loop bakwards
            it->byteIndex--;
            while((it->str->rawStr->str[it->byteIndex] & 0b11000000) == 0b10000000)
            {
                it->byteIndex--;
            }
        }
        MDPL_STDLIB_STRING_Character c;
        MDPL_RETERR(MDPL_STDLIB_STRING_getCurrent(it, &c));
        if(!utf8proc_codepoint_valid((utf8proc_int32_t)(c.codepoint)))
        {
            printf("String error: reverse iteration produced invalid code point.\n");
            return 1;
        }
        it->characterIndex += (size_t)(it->step);
        return 0;
    }
    return 0;
}
int MDPL_STDLIB_STRING_isFinished(const MDPL_STDLIB_STRING_StringIterator* const it, bool* finished)
{
    *finished = it->byteIndex >= it->str->rawStr->numBytes;
    return 0;
}

//================ Constructors ================

int MDPL_STDLIB_STRING_createStringRefFromCStr(MDPL_STDLIB_STRING_StringRef* const strRef, const char* cStr, const size_t& numBytes, const size_t& numCharacters)
{
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createStringNoRawStr(&strRef->s));
    MDPL_STDLIB_STRING_RawString* ptr;
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createRawString(&ptr, cStr, numBytes));
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_initialiseExistingString(strRef->s, numCharacters, 0, numBytes, ptr));
    return 0;
}
int MDPL_STDLIB_STRING_copyStringRef(const MDPL_STDLIB_STRING_StringRef originalStrRef, MDPL_STDLIB_STRING_StringRef* const newStrRef)
{
    *((MDPL_STDLIB_STRING_String**)&newStrRef->s) = originalStrRef.s;
    newStrRef->s->refCount++;
    return 0;
}
int MDPL_STDLIB_STRING_destroyStringRef(const MDPL_STDLIB_STRING_StringRef strRef)
{
    MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_destroyString(strRef.s));
    return 0;
}


int MDPL_STDLIB_STRING_INTERNAL_createRawString(MDPL_STDLIB_STRING_RawString* const* newStr, const char* data, const size_t& numBytes)
{
    //a size_t is required to allocate memory though the information is not needed here 
    size_t notUsed;
    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned((void**)((void* const*)(newStr)), &notUsed, 4, sizeof(MDPL_STDLIB_STRING_RawString) + numBytes));
    //initialise string
    (**newStr).refCount = 0;
    *(size_t*)&(**newStr).numBytes = numBytes;
    memcpy((char*)(**newStr).str, data, numBytes);
    return 0;
}
int MDPL_STDLIB_STRING_INTERNAL_createRawStringNoCopy(MDPL_STDLIB_STRING_RawString* const* newStr, const size_t& numBytes)
{
    //a size_t is required to allocate memory though the information is not needed here 
    size_t notUsed;
    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned((void**)((void* const*)(newStr)), &notUsed, 4, sizeof(MDPL_STDLIB_STRING_RawString) + numBytes));
    //initialise string
    (**newStr).refCount = 0;
    *(size_t*)&(**newStr).numBytes = numBytes;
    return 0;
}
int MDPL_STDLIB_STRING_INTERNAL_destroyRawString(MDPL_STDLIB_STRING_RawString* const str)
{
    MDPL_RETERR(mdpl::runtimeLib::allocator::deallocate((void*)(str)));

    return 0;
}

int MDPL_STDLIB_STRING_INTERNAL_createStringWithRawStr(MDPL_STDLIB_STRING_String* const* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, MDPL_STDLIB_STRING_RawString* const rawStr)
{
    (**newStr).numCharacters = numCharacters;
    (**newStr).startByte = startByte;
    (**newStr).endByte = endByte;
    (**newStr).flagsData = 0;
    (**newStr).flagsSet = 0;
    *(MDPL_STDLIB_STRING_RawString**)&(**newStr).rawStr = rawStr;
    (**newStr).normalisedStr = nullptr;
    (**newStr).rawStr->refCount++;
    return 0;
}
int MDPL_STDLIB_STRING_INTERNAL_createStringNoRawStr(MDPL_STDLIB_STRING_String* const* newStr)
{
    //construct the string and assign it values to make it clear that it is not initialised
    MDPL_STDLIB_STRING_String* ptr;
    size_t notUsed;
    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned((void**)(&ptr), &notUsed, 4, sizeof(MDPL_STDLIB_STRING_String)));
    ptr->refCount = 0;
    ptr->numCharacters = npos;
    ptr->startByte = npos;
    ptr->endByte = npos;
    ptr->flagsData = 0;
    ptr->flagsSet = 0;
    *(MDPL_STDLIB_STRING_RawString**)&ptr->rawStr = nullptr;
    ptr->normalisedStr = nullptr;
    //copy the string taking a reference and then manually increment refCount
    *(MDPL_STDLIB_STRING_String**)newStr = ptr;
    ptr->refCount++;
    return 0;
}
int MDPL_STDLIB_STRING_INTERNAL_initialiseExistingString(MDPL_STDLIB_STRING_String* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, MDPL_STDLIB_STRING_RawString* const rawStr)
{
    newStr->numCharacters = numCharacters;
    newStr->startByte = startByte;
    newStr->endByte = endByte;
    newStr->flagsData = 0;
    newStr->flagsSet = 0;
    *(MDPL_STDLIB_STRING_RawString**)&newStr->rawStr = rawStr;
    newStr->normalisedStr = nullptr;
    newStr->rawStr->refCount++;
    return 0;
}
int MDPL_STDLIB_STRING_INTERNAL_copyString(const MDPL_STDLIB_STRING_String* const originalStr, MDPL_STDLIB_STRING_String* const* newStr)
{
    size_t notUsed;
    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned((void**)((MDPL_STDLIB_STRING_String**)(newStr)), &notUsed, 4, sizeof(MDPL_STDLIB_STRING_String)));
    (**newStr).refCount = 1;
    (**newStr).numCharacters = originalStr->numCharacters;
    (**newStr).startByte = originalStr->startByte;
    (**newStr).endByte = originalStr->endByte;
    (**newStr).flagsData = originalStr->flagsData;
    (**newStr).flagsSet = originalStr->flagsSet;
    *(MDPL_STDLIB_STRING_RawString**)&(**newStr).rawStr = originalStr->rawStr;
    (**newStr).normalisedStr = originalStr->normalisedStr;
    (**newStr).rawStr->refCount++;
    return 0;
}
int MDPL_STDLIB_STRING_INTERNAL_destroyString(MDPL_STDLIB_STRING_String* const str)
{
    if(str->rawStr != nullptr)
    {
        str->rawStr->refCount--;
        if(str->rawStr->refCount == 0)
        {
            MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_destroyRawString(str->rawStr));
        }
    }
    if(str->normalisedStr != nullptr)
    {
        str->normalisedStr->refCount--;
        if(str->normalisedStr->refCount == 0)
        {
            MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_destroyRawString(str->normalisedStr));
        }
    }
    str->refCount--;
    if(str->refCount == 0)
    {
        MDPL_RETERR(mdpl::runtimeLib::allocator::deallocate(str));
    }
    return 0;
}

int MDPL_STDLIB_STRING_INTERNAL_normaliseString(MDPL_STDLIB_STRING_String* const str)
{
    //TODO: skip normalising if the string is ascii
    if(str->normalisedStr == nullptr)
    {
        const utf8proc_uint8_t* originalCStr = (const utf8proc_uint8_t*)(str->rawStr->str + str->startByte);
        const utf8proc_ssize_t originalNumBytes = (utf8proc_ssize_t)(str->rawStr->numBytes);

        //the following code is a moddified version of the function utf8proc_map_custom in vendor/utf8proc/utf8proc.c.
        //as there are no comments in the original this is my best attempt at explaining what is happening

        //the options bitset instructs utf8proc as to what we are trying to do
        const utf8proc_option_t options = (utf8proc_option_t)(UTF8PROC_STABLE | UTF8PROC_COMPOSE);
        //to be able to normalise a string we must first decompose it and to do this we need to know how many bytes are needed.
        //calling the function with the third and fourth parameters set to nullptr and 0 respectivly queries the length with out performing any conversion
        utf8proc_ssize_t result = utf8proc_decompose_custom(originalCStr, originalNumBytes, nullptr, 0, options, nullptr, nullptr);
        if(result < 0)
        {
            printf("String error: \"%s\" occured during counting number of bytes for normalisation.\n", utf8proc_errmsg(result));
            return 1;
        }
        //for reasons I don't understand we must assume that each bytes will be the maximum length of 4 bytes. An extra byte is added for null terminating the string to determine it's length.
        size_t numBytesRequiredForDecomposition = (utf8proc_size_t)(result) * sizeof(utf8proc_int32_t);
        //while the final string will be shorter than or equal to numBytesRequiredForDecomposition we require a temporary buffer. To avoid reallocation this will be the final output.
        MDPL_RETERR(MDPL_STDLIB_STRING_INTERNAL_createRawStringNoCopy(&str->normalisedStr, numBytesRequiredForDecomposition));
        utf8proc_int32_t* buffer = (utf8proc_int32_t*)((char*)(str->normalisedStr->str));
        //I have no idea what result represents
        //calling the function with the third and fourth parameters set to as follows performs the decompistion
        result = utf8proc_decompose_custom(originalCStr, originalNumBytes, buffer, result, options, nullptr, nullptr);
        if(result < 0)
        {
            printf("String error: \"%s\" occured during decomposing string for normalisation.\n", utf8proc_errmsg(result));
            return 1;
        }
        //I have no idea what result represents
        //this functions takes the fully decomposed string and composes it into the normalised form
        result = utf8proc_reencode(buffer, result, options);
        if(result < 0)
        {
            printf("String error: \"%s\" occured during composing string for normalisation.\n", utf8proc_errmsg(result));
            return 1;
        }
        //finish initialising the string
        str->normalisedStr->refCount++;
        *(size_t*)&str->normalisedStr->numBytes = (size_t)result;
    }

    return 0;
}

int MDPL_STDLIB_STRING_INTERNAL_createInternalIterator(const MDPL_STDLIB_STRING_StringRef str, const utf8proc_uint8_t** ptr)
{
    *ptr = (const utf8proc_uint8_t*)(str.s->rawStr->str) + str.s->startByte;
    return 0;
}
int MDPL_STDLIB_STRING_INTERNAL_incrementInternalIterator(const MDPL_STDLIB_STRING_StringRef str, const utf8proc_uint8_t** ptr, utf8proc_int32_t* character)
{
    if(*ptr < (const utf8proc_uint8_t*)(str.s->rawStr->str) + str.s->endByte)
    {
        utf8proc_ssize_t retcode = utf8proc_iterate(*ptr, -1, character);
        if(retcode < 0)
        {
            printf("String error: \"%s\" occured during incrementInternalIterator.\n", utf8proc_errmsg(retcode));
            return 1;
        }
        *ptr += retcode;
    }
    else
    {
        printf("String error: Internal iterator exceeded bounds of string.\n");
        return 1;
    }
    return 0;
}
