#include "string.hpp"

namespace mdpl
{
    namespace standardLibrary
    {
        namespace String
        {
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

                int createStringWithRawStr(String** newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr)
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
                        const utf8proc_uint8_t* originalCStr = reinterpret_cast<const utf8proc_uint8_t*>(str->rawStr->str);
                        const utf8proc_ssize_t originalNumBytes = static_cast<utf8proc_ssize_t>(str->rawStr->numBytes);

                        //the following code is a moddified version of the function utf8proc_map_custom in vendor/utf8proc/utf8proc.c.
                        //as there are no comments in the original this is my best attempt at explaining what is happening

                        //the options bitset instructs utf8proc as to what we are trying to do
                        const utf8proc_option_t options = static_cast<utf8proc_option_t>(UTF8PROC_STABLE | UTF8PROC_COMPOSE);
                        //to be able to normalise a string we must first decompose it and to do this we need to know how many characters are needed. Note these are unicode characters which may not nessesarily be printable.
                        //calling the function with the third and fourth parameters set to nullptr and 0 respectivly queries the length with out performing any conversion
                        utf8proc_ssize_t result = utf8proc_decompose_custom(originalCStr, originalNumBytes, nullptr, 0, options, nullptr, nullptr);
                        if(result < 0)
                        {
                            printf("String error: \"%s\" occured during counting number of characters for normalisation.\n", utf8proc_errmsg(result));
                            return 1;
                        }
                        //for reasons I don't understand we must assume that each character will be the maximum length of 4 code points / 4 bytes. An extra byte is added for null terminating the string to determine it's length.
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
            }
        }
    }
}
