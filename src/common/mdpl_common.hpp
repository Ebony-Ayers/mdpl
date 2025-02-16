#ifndef MDPL_COMMON_HEADER_GUARD
#define MDPL_COMMON_HEADER_GUARD

#include "system_headers.hpp"
#include "file_access.hpp"
#include "buffer.hpp"
#include "str_functions.hpp"

//return if the integer return code is non-zero
#define MDPL_RETERR(rc) {int MDPL_RETCODE; if((MDPL_RETCODE = rc) != 0) return MDPL_RETCODE;}

namespace mdpl
{
    namespace common
    {
        template<typename T>
        void writeToPointerConst(T* const* ptr, T* const* value)
        {
            //memcpy(const_cast<void*>(reinterpret_cast<const void*>(ptr)), reinterpret_cast<const void*>(value), sizeof(void*));
            *(const_cast<void**>(reinterpret_cast<void* const*>(ptr))) = *(const_cast<void**>(reinterpret_cast<void* const*>(value)));
        }

        template<typename T>
        void writeNullToPointerConst(T* const* ptr)
        {
            //T* value = nullptr;
            //writeToPointerConst(ptr, value);
            *(const_cast<void**>(reinterpret_cast<void* const*>(ptr))) = nullptr;
        }

        template<typename T>
        void copyToConstArray(const T* dst, const T* src, const size_t& numBytes)
        {
            memcpy(const_cast<void*>(reinterpret_cast<const void*>(dst)), reinterpret_cast<const void*>(src), numBytes);
        }

        template<typename T>
        void writeToConstVariable(const T* var, const T& value)
        {
            const_cast<T&>(*var) = value;
        }
    }
}

#endif //MDPL_COMMON_HEADER_GUARD