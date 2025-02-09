#include "string.hpp"

namespace mdpl
{
    namespace standardLibrary
    {
        namespace String
        {
            //================ Internal functions ================

            int createRawString(RawString** newStr, const char* data, const size_t& numBytes)
            {
                //a size_t is required to allocate memory though the information is not needed here 
                size_t notUsed;
                MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned(reinterpret_cast<void**>(newStr), &notUsed, 4, sizeof(RawString) + numBytes));
                //initialise string
                (**newStr).refCount = 0;
                mdpl::common::writeToConstVariable(&(**newStr).numBytes, numBytes);
                (**newStr).flags = 0;
                mdpl::common::copyToConstArray((**newStr).str, data, numBytes);
                return 0;
            }
            int createRawStringNoCopy(RawString** newStr, const size_t& numBytes)
            {
                //a size_t is required to allocate memory though the information is not needed here 
                size_t notUsed;
                MDPL_RETERR(mdpl::runtimeLib::allocator::allocateAlligned(reinterpret_cast<void**>(newStr), &notUsed, 4, sizeof(RawString) + numBytes));
                //initialise string
                (**newStr).refCount = 0;
                mdpl::common::writeToConstVariable(&(**newStr).numBytes, numBytes);
                (**newStr).flags = 0;
                return 0;
            }
            int destroyRawString(RawString* const str)
            {
                MDPL_RETERR(mdpl::runtimeLib::allocator::deallocate(reinterpret_cast<void*>(str)));

                return 0;
            }

            int createString(String* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr)
            {
                newStr->numCharacters = numCharacters;
                newStr->startByte = startByte;
                newStr->endByte = endByte;
                mdpl::common::writeToPointerConst(&newStr->rawStr, &rawStr);
                newStr->normalisedStr = nullptr;
                newStr->rawStr->refCount++;
                return 0;
            }
            int copyString(const String* const originalStr, String* newStr)
            {
                newStr->numCharacters = originalStr->numCharacters;
                newStr->startByte = originalStr->startByte;
                newStr->endByte = originalStr->endByte;
                mdpl::common::writeToPointerConst(&newStr->rawStr, &originalStr->rawStr);
                newStr->normalisedStr = originalStr->normalisedStr;
                newStr->rawStr->refCount++;
                if(newStr->normalisedStr != nullptr)
                {
                    newStr->normalisedStr->refCount++;
                }
                return 0;
            }
            int destroyString(String* const str)
            {
                str->rawStr->refCount--;
                if(str->rawStr->refCount == 0)
                {
                    MDPL_RETERR(destroyRawString(str->rawStr));
                }
                if(str->normalisedStr != nullptr)
                {
                    str->normalisedStr->refCount--;
                    if(str->normalisedStr->refCount == 0)
                    {
                        MDPL_RETERR(destroyRawString(str->normalisedStr));
                    }
                }
                return 0;
            }

            int normaliseString(String* const str)
            {
                if(str->normalisedStr == nullptr)
                {
                    //the following code is a moddified version of the function utf8proc_map_custom in vendor/utf8proc/utf8proc.c.
                    //as there are no comments in the original this is my best attempt at explaining what is happening

                    //the options bitset instructs utf8proc as to what we are trying to do
                    const utf8proc_option_t options = static_cast<utf8proc_option_t>(UTF8PROC_STABLE | UTF8PROC_COMPOSE);
                    //to be able to normalise a string we must first decompose it and to do this we need to know how many characters are needed. Note these are unicode characters which may not nessesarily be printable.
                    //calling the function with the third and fourth parameters set to nullptr and 0 respectivly queries the length with out performing any conversion
                    utf8proc_ssize_t numDecomposedUnicodeCharacters = utf8proc_decompose_custom(reinterpret_cast<const utf8proc_uint8_t*>(str->rawStr->str), str->rawStr->numBytes, nullptr, 0, options, nullptr, nullptr);
                    if(numDecomposedUnicodeCharacters < 0)
                    {
                        printf("String error: \"%s\" occured during counting number of characters for normalisation.\n", utf8proc_errmsg(numDecomposedUnicodeCharacters));
                        return 1;
                    }
                    //for reasons I don't understand we must assume that each character will be the maximum length of 4 code points / 4 bytes. An extra byte is added for null terminating the string to determine it's length.
                    size_t numBytesRequiredForDecomposition = static_cast<size_t>(numDecomposedUnicodeCharacters) * sizeof(utf8proc_int32_t) + 1;
                    //while the final string will be shorter than or equal to numBytesRequiredForDecomposition we require a temporary buffer. To avoid reallocation this will be the final output.
                    MDPL_RETERR(createRawStringNoCopy(&str->normalisedStr, numBytesRequiredForDecomposition));
                    utf8proc_int32_t* buffer = reinterpret_cast<utf8proc_int32_t*>(const_cast<char*>(str->normalisedStr->str));
                    reinterpret_cast<char*>(buffer)[numBytesRequiredForDecomposition - 1] = '\0';
                    //I have no idea what result represents
                    //calling the function with the third and fourth parameters set to as follows performs the decompistion
                    utf8proc_ssize_t result = utf8proc_decompose_custom(reinterpret_cast<const utf8proc_uint8_t*>(str->rawStr->str), str->rawStr->numBytes, buffer, numDecomposedUnicodeCharacters, options, nullptr, nullptr);
                    if(result < 0)
                    {
                        printf("String error: \"%s\" occured during decomposing string for normalisation.\n", utf8proc_errmsg(numDecomposedUnicodeCharacters));
                        return 1;
                    }
                    //I have no idea what result represents
                    //this functions takes the fully decomposed string and composes it into the normalised form
                    result = utf8proc_reencode(buffer, result, options);
                    if(result < 0)
                    {
                        printf("String error: \"%s\" occured during composing string for normalisation.\n", utf8proc_errmsg(numDecomposedUnicodeCharacters));
                        return 1;
                    }
                    //finish initialising the string
                    str->normalisedStr->refCount++;
                    mdpl::common::writeToConstVariable(&str->normalisedStr->numBytes, static_cast<size_t>(strlen(str->normalisedStr->str)));
                    str->normalisedStr->flags |= RawStringFlags::isNormalised;
                }

                return 0;
            }
        }
    }
}
