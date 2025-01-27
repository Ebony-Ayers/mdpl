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
                const_cast<size_t&>((**newStr).numBytes) = numBytes;
                (**newStr).flags = 0;
                memcpy(const_cast<void*>(reinterpret_cast<const void*>((**newStr).str)), reinterpret_cast<const void*>(data), numBytes);
                return 0;
            }
            int destroyRawString(RawString* const str)
            {
                MDPL_RETERR(mdpl::runtimeLib::allocator::deallocate(reinterpret_cast<void*>(str)));

                return 0;
            }

            int createString(String* newStr, const size_t& numCharacters, const size_t& startByte, const size_t& endByte, RawString* const rawStr)
            {
                (*newStr).numCharacters = numCharacters;
                (*newStr).startByte = startByte;
                (*newStr).endByte = endByte;
                memcpy(const_cast<void*>(reinterpret_cast<const void*>(&(newStr->rawStr))), reinterpret_cast<const void*>(&rawStr), sizeof(void*));
                (*newStr).rawStr->refCount++;
                return 0;
            }
            int copyString(const String* const originalStr, String* newStr)
            {
                (*newStr).numCharacters = originalStr->numCharacters;
                (*newStr).startByte = originalStr->startByte;
                (*newStr).endByte = originalStr->endByte;
                memcpy(const_cast<void*>(reinterpret_cast<const void*>(&(newStr->rawStr))), const_cast<void*>(reinterpret_cast<const void*>(&(originalStr->rawStr))), sizeof(void*));
                (*newStr).rawStr->refCount++;
                return 0;
            }
            int destroyString(String* const str)
            {
                str->rawStr->refCount--;
                if(str->rawStr->refCount == 0)
                {
                    MDPL_RETERR(destroyRawString(str->rawStr));
                }
                return 0;
            }
        }
    }
}
