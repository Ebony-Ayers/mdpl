#include "string.hpp"

namespace mdpl
{
    namespace standardLibrary
    {
        namespace String
        {
            //================ String functions ================

            int isLower(const StringRef str, bool* result)
            {
                if(str.s->flagsSet & StringFlags::isLower) [[likely]]
                {
                    *result = str.s->flagsData & StringFlags::isLower ? true : false;
                    return 0;
                }

                *result = true;
                const utf8proc_uint8_t* ptr;
                MDPL_RETERR(internal::createInternalIterator(str, &ptr));
                for (size_t i = 0; i < str.s->numCharacters; i++)
                {
                    utf8proc_int32_t character;
                    MDPL_RETERR(internal::incrementInternalIterator(str, &ptr, &character));
                    const utf8proc_property_t *p = utf8proc_get_property(character);
                    if((p->lowercase_seqindex != UINT16_MAX) && (p->uppercase_seqindex == UINT16_MAX))
                    {
                        *result = false;
                        str.s->flagsSet |= StringFlags::isLower;
                        str.s->flagsData &= ~StringFlags::isLower;
                        return 0;
                    }
                }
                str.s->flagsSet |= StringFlags::isLower;
                str.s->flagsData |= StringFlags::isLower;
                return 0;
            }
            int isUpper(const StringRef str, bool* result)
            {
                if(str.s->flagsSet & StringFlags::isUpper) [[likely]]
                {
                    *result = str.s->flagsData & StringFlags::isUpper ? true : false;
                    return 0;
                }

                *result = true;
                const utf8proc_uint8_t* ptr;
                MDPL_RETERR(internal::createInternalIterator(str, &ptr));
                for (size_t i = 0; i < str.s->numCharacters; i++)
                {
                    utf8proc_int32_t character;
                    MDPL_RETERR(internal::incrementInternalIterator(str, &ptr, &character));
                    const utf8proc_property_t *p = utf8proc_get_property(character);
                    if((p->lowercase_seqindex == UINT16_MAX) && (p->uppercase_seqindex != UINT16_MAX))
                    {
                        *result = false;
                        str.s->flagsSet |= StringFlags::isUpper;
                        str.s->flagsData &= ~StringFlags::isUpper;
                        return 0;
                    }
                }
                str.s->flagsSet |= StringFlags::isUpper;
                str.s->flagsData |= StringFlags::isUpper;
                return 0;
            }
            int isWhiteSpace(const StringRef str, bool* result)
            {
                if(str.s->flagsSet & StringFlags::isWhiteSpace) [[likely]]
                {
                    *result = str.s->flagsData & StringFlags::isWhiteSpace ? true : false;
                    return 0;
                }

                *result = true;
                const utf8proc_uint8_t* ptr;
                MDPL_RETERR(internal::createInternalIterator(str, &ptr));
                for (size_t i = 0; i < str.s->numCharacters; i++)
                {
                    utf8proc_int32_t character;
                    MDPL_RETERR(internal::incrementInternalIterator(str, &ptr, &character));
                    const utf8proc_property_t *p = utf8proc_get_property(character);
                    if(p->bidi_class != UTF8PROC_BIDI_CLASS_WS)
                    {
                        *result = false;
                        str.s->flagsSet |= StringFlags::isWhiteSpace;
                        str.s->flagsData &= ~StringFlags::isWhiteSpace;
                        return 0;
                    }
                }
                str.s->flagsSet |= StringFlags::isWhiteSpace;
                str.s->flagsData |= StringFlags::isWhiteSpace;
                return 0;
            }
            int isPrintable(const StringRef str, bool* result)
            {
                if(str.s->flagsSet & StringFlags::isPrintable) [[likely]]
                {
                    *result = str.s->flagsData & StringFlags::isPrintable ? true : false;
                    return 0;
                }

                *result = true;
                const utf8proc_uint8_t* ptr;
                MDPL_RETERR(internal::createInternalIterator(str, &ptr));
                for (size_t i = 0; i < str.s->numCharacters; i++)
                {
                    utf8proc_int32_t character;
                    MDPL_RETERR(internal::incrementInternalIterator(str, &ptr, &character));
                    //a char width of 0 indicates the character is non-printable
                    if(utf8proc_charwidth(character) == 0)
                    {
                        *result = false;
                        str.s->flagsSet |= StringFlags::isPrintable;
                        str.s->flagsData &= ~StringFlags::isPrintable;
                        return 0;
                    }
                }
                str.s->flagsSet |= StringFlags::isPrintable;
                str.s->flagsData |= StringFlags::isPrintable;
                return 0;
            }
            int isAscii(const StringRef str, bool* result)
            {
                if(str.s->flagsSet & StringFlags::isAscii) [[likely]]
                {
                    *result = str.s->flagsData & StringFlags::isAscii ? true : false;
                    return 0;
                }
                else
                {
                    for(size_t i = str.s->startByte; i < str.s->endByte; i++)
                    {
                        if(str.s->rawStr->str[i] & 0b10000000)
                        {
                            *result = false;
                            str.s->flagsSet |= StringFlags::isAscii;
                            str.s->flagsData &= ~StringFlags::isAscii;
                            return 0;
                        }
                    }
                    *result = true;
                    str.s->flagsSet |= StringFlags::isAscii;
                    str.s->flagsData |= StringFlags::isAscii;
                    return 0;
                }
            }
            int isValidDecimal(const StringRef str, bool* result)
            {
                //check if a previous call to this function has been done
                if(str.s->flagsSet & StringFlags::isValidDecimal)
                {
                    *result = str.s->flagsData & StringFlags::isValidDecimal ? true : false;
                    return 0;
                }
                //if the stirng is not ascii then by definition it cannot be a decimal
                bool ascii;
                MDPL_RETERR(isAscii(str, &ascii));
                if(!ascii)
                {
                    *result = false;
                    str.s->flagsSet |= StringFlags::isValidDecimal;
                    str.s->flagsData &= ~StringFlags::isValidDecimal;
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
                            str.s->flagsSet |= StringFlags::isValidDecimal;
                            str.s->flagsData &= ~StringFlags::isValidDecimal;
                            return 0;
                        }
                    }
                    *result = true;
                    str.s->flagsSet |= StringFlags::isValidDecimal;
                    str.s->flagsData |= StringFlags::isValidDecimal;
                    return 0;
                }
            }
            int isValidInt(const StringRef str, bool* result)
            {
                //check if a previous call to this function has been done
                if(str.s->flagsSet & StringFlags::isValidInt)
                {
                    *result = str.s->flagsData & StringFlags::isValidInt ? true : false;
                    return 0;
                }
                //if the stirng is not ascii then by definition it cannot be an int
                bool ascii;
                MDPL_RETERR(isAscii(str, &ascii));
                if(!ascii)
                {
                    *result = false;
                    str.s->flagsSet |= StringFlags::isValidInt;
                    str.s->flagsData &= ~StringFlags::isValidInt;
                    return 0;
                }
                else
                {
                    //if the first character is a - then we can skip checking it
                    const size_t firstCharacterHythenOffset = str.s->rawStr->str[str.s->startByte] == '-' ? 1 : 0;
                    for(size_t i = str.s->startByte + firstCharacterHythenOffset; i < str.s->endByte; i++)
                    {
                        const char c = str.s->rawStr->str[i];
                        if(! ((c >= '0') && (c <= '9')) )
                        {
                            *result = false;
                            str.s->flagsSet |= StringFlags::isValidInt;
                            str.s->flagsData &= ~StringFlags::isValidInt;
                            return 0;
                        }
                    }
                    *result = true;
                    str.s->flagsSet |= StringFlags::isValidInt;
                    str.s->flagsData |= StringFlags::isValidInt;
                    return 0;
                }
            }
            int isValidFloat(const StringRef str, bool* result)
            {
                //check if a previous call to this function has been done
                if(str.s->flagsSet & StringFlags::isValidFloat)
                {
                    *result = str.s->flagsData & StringFlags::isValidFloat ? true : false;
                    return 0;
                }
                //if the stirng is not ascii then by definition it cannot be an int
                bool ascii;
                MDPL_RETERR(isAscii(str, &ascii));
                if(!ascii)
                {
                    *result = false;
                    str.s->flagsSet |= StringFlags::isValidFloat;
                    str.s->flagsData &= ~StringFlags::isValidFloat;
                    return 0;
                }
                else
                {
                    //if the first character is a - then we can skip checking it
                    const size_t firstCharacterHythenOffset = str.s->rawStr->str[str.s->startByte] == '-' ? 1 : 0;
                    bool alreadyFoundDecimalPoint = false;
                    for(size_t i = str.s->startByte + firstCharacterHythenOffset; i < str.s->endByte; i++)
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
                                    str.s->flagsSet |= StringFlags::isValidFloat;
                                    str.s->flagsData &= ~StringFlags::isValidFloat;
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
                                str.s->flagsSet |= StringFlags::isValidFloat;
                                str.s->flagsData &= ~StringFlags::isValidFloat;
                                return 0;
                            }
                        }
                    }
                    *result = true;
                    str.s->flagsSet |= StringFlags::isValidFloat;
                    str.s->flagsData |= StringFlags::isValidFloat;
                    return 0;
                }
            }
            int isAlpha(const StringRef str, bool* result)
            {
                if(str.s->flagsSet & StringFlags::isAlpha) [[likely]]
                {
                    *result = str.s->flagsData & StringFlags::isAlpha ? true : false;
                    return 0;
                }

                *result = true;
                const utf8proc_uint8_t* ptr;
                MDPL_RETERR(internal::createInternalIterator(str, &ptr));
                for (size_t i = 0; i < str.s->numCharacters; i++)
                {
                    utf8proc_int32_t character;
                    MDPL_RETERR(internal::incrementInternalIterator(str, &ptr, &character));
                    const utf8proc_property_t *p = utf8proc_get_property(character);
                    const int mask = UTF8PROC_CATEGORY_LU | UTF8PROC_CATEGORY_LL | UTF8PROC_CATEGORY_LT | UTF8PROC_CATEGORY_LM | UTF8PROC_CATEGORY_LO;
                    //check if the category is a subset of the mask
                    if(((p->category & mask) == 0) || ((p->category & (~mask)) != 0))
                    {
                        *result = false;
                        str.s->flagsSet |= StringFlags::isAlpha;
                        str.s->flagsData &= ~StringFlags::isAlpha;
                        return 0;
                    }
                }
                str.s->flagsSet |= StringFlags::isAlpha;
                str.s->flagsData |= StringFlags::isAlpha;
                return 0;
            }
            int isAlphaNumeric(const StringRef str, bool* result)
            {
                {
                    if(str.s->flagsSet & StringFlags::isAlphaNumeric) [[likely]]
                    {
                        *result = str.s->flagsData & StringFlags::isAlphaNumeric ? true : false;
                        return 0;
                    }
    
                    *result = true;
                    const utf8proc_uint8_t* ptr;
                    MDPL_RETERR(internal::createInternalIterator(str, &ptr));
                    for (size_t i = 0; i < str.s->numCharacters; i++)
                    {
                        utf8proc_int32_t character;
                        MDPL_RETERR(internal::incrementInternalIterator(str, &ptr, &character));
                        const utf8proc_property_t *p = utf8proc_get_property(character);
                        const int mask = UTF8PROC_CATEGORY_LU | UTF8PROC_CATEGORY_LL | UTF8PROC_CATEGORY_LT | UTF8PROC_CATEGORY_LM | UTF8PROC_CATEGORY_LO | UTF8PROC_CATEGORY_ND | UTF8PROC_CATEGORY_NL | UTF8PROC_CATEGORY_NO;
                        //check if the category is a subset of the mask
                        if(((p->category & mask) == 0) || ((p->category & (~mask)) != 0))
                        {
                            *result = false;
                            str.s->flagsSet |= StringFlags::isAlphaNumeric;
                            str.s->flagsData &= ~StringFlags::isAlphaNumeric;
                            return 0;
                        }
                    }
                    str.s->flagsSet |= StringFlags::isAlphaNumeric;
                    str.s->flagsData |= StringFlags::isAlphaNumeric;
                    return 0;
                }
            }
            
            int substrIndex(const StringRef originalStr, StringRef* const newStr, const size_t& startIndex, const size_t& endIndex)
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

                MDPL_RETERR(internal::copyString(originalStr.s, &newStr->s));
                newStr->s->numCharacters = endIndex - startIndex;
                
                bool result;
                MDPL_RETERR(isAscii(originalStr, &result));
                if(result)
                {
                    newStr->s->startByte = startIndex;
                    newStr->s->endByte = endIndex;
                }
                else
                {
                    const utf8proc_uint8_t* startIt;
                    const utf8proc_uint8_t* movingvIt;
                    MDPL_RETERR(internal::createInternalIterator(originalStr, &startIt));
                    MDPL_RETERR(internal::createInternalIterator(originalStr, &movingvIt));
                    for(size_t i = 0; i < startIndex; i++) { utf8proc_int32_t c; MDPL_RETERR(internal::incrementInternalIterator(originalStr, &movingvIt, &c)); }
                    newStr->s->startByte = static_cast<size_t>(movingvIt - startIt);
                    for(size_t i = startIndex; i < endIndex; i++) { utf8proc_int32_t c; MDPL_RETERR(internal::incrementInternalIterator(originalStr, &movingvIt, &c)); }
                    newStr->s->endByte = static_cast<size_t>(movingvIt - startIt);
                }
                const uint32_t carryOverFlags = originalStr.s->flagsSet & originalStr.s->flagsData;
                newStr->s->flagsData = carryOverFlags;
                newStr->s->flagsSet = carryOverFlags; 
                newStr->s->normalisedStr = nullptr;
                return 0;
            }
            int substrIterator(const StringRef originalStr, StringRef* const newStr, const StringIterator& startIt, const StringIterator& endIt)
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
                MDPL_RETERR(internal::copyString(originalStr.s, &newStr->s));
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

            //================ String iterator ================

            int getCurrent(const StringIterator* const it, Character* dst)
            {
                utf8proc_ssize_t retcode = utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(it->str->rawStr->str + it->byteIndex), -1, reinterpret_cast<utf8proc_int32_t*>(&(dst->character)));
                if(retcode < 0)
                {
                    printf("String error: \"%s\" occured during string iterator getCurrrent.\n", utf8proc_errmsg(retcode));
                    return 1;
                }
                return 0;
            }
            int next(StringIterator* it)
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
                        utf8proc_ssize_t retcode = utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(it->str->rawStr->str + it->byteIndex), -1, &chracter);
                        if(retcode < 0)
                        {
                            printf("String error: \"%s\" occured during string iterator next.\n", utf8proc_errmsg(retcode));
                            return 1;
                        }
                        it->byteIndex += static_cast<size_t>(retcode);
                    }
                    it->characterIndex += static_cast<size_t>(it->step);
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
                    Character c;
                    MDPL_RETERR(getCurrent(it, &c));
                    if(!utf8proc_codepoint_valid(static_cast<utf8proc_int32_t>(c.character)))
                    {
                        printf("String error: reverse iteration produced invalid code point.\n");
                        return 1;
                    }
                    it->characterIndex += static_cast<size_t>(it->step);
                    return 0;
                }
                return 0;
            }
            int isFinished(const StringIterator* const it, bool* finished)
            {
                *finished = it->byteIndex >= it->str->rawStr->numBytes;
                return 0;
            }
        
            //================ Constructors ================

            int createStringRefFromCStr(StringRef* const strRef, const char* cStr, const size_t& numBytes, const size_t& numCharacters)
            {
                MDPL_RETERR(internal::createStringNoRawStr(&strRef->s));
                RawString* ptr;
                MDPL_RETERR(internal::createRawString(&ptr, cStr, numBytes));
                MDPL_RETERR(internal::initialiseExistingString(strRef->s, numCharacters, 0, numBytes, ptr));
                return 0;
            }
            int copyStringRef(const StringRef originalStrRef, StringRef* const newStrRef)
            {
                mdpl::common::writeToPointerConst(&newStrRef->s, &originalStrRef.s);
                newStrRef->s->refCount++;
                return 0;
            }
            int destroyStringRef(const StringRef strRef)
            {
                MDPL_RETERR(internal::destroyString(strRef.s));
                return 0;
            }

            namespace internal
            {
                int createRawString(RawString* const* newStr, const char* data, const size_t& numBytes)
                {
                    //a size_t is required to allocate memory though the information is not needed here 
                    size_t notUsed;
                    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned(const_cast<void**>(reinterpret_cast<void* const*>(newStr)), &notUsed, 4, sizeof(RawString) + numBytes));
                    //initialise string
                    (**newStr).refCount = 0;
                    mdpl::common::writeToConstVariable(&(**newStr).numBytes, numBytes);
                    mdpl::common::copyToConstArray((**newStr).str, data, numBytes);
                    return 0;
                }
                int createRawStringNoCopy(RawString* const* newStr, const size_t& numBytes)
                {
                    //a size_t is required to allocate memory though the information is not needed here 
                    size_t notUsed;
                    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned(const_cast<void**>(reinterpret_cast<void* const*>(newStr)), &notUsed, 4, sizeof(RawString) + numBytes));
                    //initialise string
                    (**newStr).refCount = 0;
                    mdpl::common::writeToConstVariable(&(**newStr).numBytes, numBytes);
                    return 0;
                }
                int destroyRawString(RawString* const str)
                {
                    MDPL_RETERR(mdpl::runtimeLib::allocator::deallocate(reinterpret_cast<void*>(str)));

                    return 0;
                }

                int createStringWithRawStr(String* const* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr)
                {
                    (**newStr).numCharacters = numCharacters;
                    (**newStr).startByte = startByte;
                    (**newStr).endByte = endByte;
                    (**newStr).flagsData = 0;
                    (**newStr).flagsSet = 0;
                    mdpl::common::writeToPointerConst(&(**newStr).rawStr, &rawStr);
                    (**newStr).normalisedStr = nullptr;
                    (**newStr).rawStr->refCount++;
                    return 0;
                }
                int createStringNoRawStr(String* const* newStr)
                {
                    //construct the string and assign it values to make it clear that it is not initialised
                    String* ptr;
                    size_t notUsed;
                    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned(reinterpret_cast<void**>(&ptr), &notUsed, 4, sizeof(String)));
                    ptr->refCount = 0;
                    ptr->numCharacters = npos;
                    ptr->startByte = npos;
                    ptr->endByte = npos;
                    ptr->flagsData = 0;
                    ptr->flagsSet = 0;
                    mdpl::common::writeNullToPointerConst(&ptr->rawStr);
                    ptr->normalisedStr = nullptr;
                    //take a reference to the string
                    mdpl::common::writeToPointerConst(newStr, &ptr);
                    ptr->refCount++;
                    return 0;
                }
                int initialiseExistingString(String* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr)
                {
                    newStr->numCharacters = numCharacters;
                    newStr->startByte = startByte;
                    newStr->endByte = endByte;
                    newStr->flagsData = 0;
                    newStr->flagsSet = 0;
                    mdpl::common::writeToPointerConst(&newStr->rawStr, &rawStr);
                    newStr->normalisedStr = nullptr;
                    newStr->rawStr->refCount++;
                    return 0;
                }
                int copyString(const String* const originalStr, String* const* newStr)
                {
                    size_t notUsed;
                    MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned(reinterpret_cast<void**>(const_cast<String**>(newStr)), &notUsed, 4, sizeof(String)));
                    (**newStr).refCount = 1;
                    (**newStr).numCharacters = originalStr->numCharacters;
                    (**newStr).startByte = originalStr->startByte;
                    (**newStr).endByte = originalStr->endByte;
                    (**newStr).flagsData = originalStr->flagsData;
                    (**newStr).flagsSet = originalStr->flagsSet;
                    mdpl::common::writeToPointerConst(&(**newStr).rawStr, &(originalStr->rawStr));
                    (**newStr).normalisedStr = originalStr->normalisedStr;
                    (**newStr).rawStr->refCount++;
                    return 0;
                }
                int destroyString(String* const str)
                {
                    if(str->rawStr != nullptr)
                    {
                        str->rawStr->refCount--;
                        if(str->rawStr->refCount == 0)
                        {
                            MDPL_RETERR(destroyRawString(str->rawStr));
                        }
                    }
                    if(str->normalisedStr != nullptr)
                    {
                        str->normalisedStr->refCount--;
                        if(str->normalisedStr->refCount == 0)
                        {
                            MDPL_RETERR(destroyRawString(str->normalisedStr));
                        }
                    }
                    str->refCount--;
                    if(str->refCount == 0)
                    {
                        MDPL_RETERR(mdpl::runtimeLib::allocator::deallocate(str));
                    }
                    return 0;
                }

                int normaliseString(String* const str)
                {
                    if(str->normalisedStr == nullptr)
                    {
                        const utf8proc_uint8_t* originalCStr = reinterpret_cast<const utf8proc_uint8_t*>(str->rawStr->str + str->startByte);
                        const utf8proc_ssize_t originalNumBytes = static_cast<utf8proc_ssize_t>(str->rawStr->numBytes);

                        //the following code is a moddified version of the function utf8proc_map_custom in vendor/utf8proc/utf8proc.c.
                        //as there are no comments in the original this is my best attempt at explaining what is happening

                        //the options bitset instructs utf8proc as to what we are trying to do
                        const utf8proc_option_t options = static_cast<utf8proc_option_t>(UTF8PROC_STABLE | UTF8PROC_COMPOSE);
                        //to be able to normalise a string we must first decompose it and to do this we need to know how many bytes are needed.
                        //calling the function with the third and fourth parameters set to nullptr and 0 respectivly queries the length with out performing any conversion
                        utf8proc_ssize_t result = utf8proc_decompose_custom(originalCStr, originalNumBytes, nullptr, 0, options, nullptr, nullptr);
                        if(result < 0)
                        {
                            printf("String error: \"%s\" occured during counting number of bytes for normalisation.\n", utf8proc_errmsg(result));
                            return 1;
                        }
                        //for reasons I don't understand we must assume that each bytes will be the maximum length of 4 bytes. An extra byte is added for null terminating the string to determine it's length.
                        size_t numBytesRequiredForDecomposition = static_cast<utf8proc_size_t>(result) * sizeof(utf8proc_int32_t);
                        //while the final string will be shorter than or equal to numBytesRequiredForDecomposition we require a temporary buffer. To avoid reallocation this will be the final output.
                        MDPL_RETERR(mdpl::standardLibrary::String::internal::createRawStringNoCopy(&str->normalisedStr, numBytesRequiredForDecomposition));
                        utf8proc_int32_t* buffer = reinterpret_cast<utf8proc_int32_t*>(const_cast<char*>(str->normalisedStr->str));
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
                        mdpl::common::writeToConstVariable(&str->normalisedStr->numBytes, static_cast<size_t>(result));
                    }

                    return 0;
                }

                int createInternalIterator(const StringRef str, const utf8proc_uint8_t** ptr)
                {
                    *ptr = reinterpret_cast<const utf8proc_uint8_t*>(str.s->rawStr->str) + str.s->startByte;
                    return 0;
                }
                int incrementInternalIterator(const StringRef str, const utf8proc_uint8_t** ptr, utf8proc_int32_t* character)
                {
                    if(*ptr < reinterpret_cast<const utf8proc_uint8_t*>(str.s->rawStr->str) + str.s->endByte)
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
            }
        }
    }
}
