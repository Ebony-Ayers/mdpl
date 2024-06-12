#ifndef MDPL_BUFFER_HEADER_BUARD
#define MDPL_BUFFER_HEADER_BUARD

#include "../pch.hpp"

namespace mdpl
{
    namespace common
    {
        template<typename T>
        class RAIIBuffer
        {
            public:
                RAIIBuffer();
                ~RAIIBuffer();

                operator T*() const;
                T* getBuff() const;

                int allocate(size_t n);
                int reallocate(size_t n);
            
            private:
                T* m_buff;
        };
    }
}

#include "buffer.inl"

#endif //MDPL_BUFFER_HEADER_BUARD