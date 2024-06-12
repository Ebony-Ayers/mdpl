#include "buffer.hpp"

namespace mdpl
{
    namespace common
    {
        template<typename T>
        RAIIBuffer<T>::RAIIBuffer()
        : m_buff(nullptr)
        {}
        template<typename T>
        RAIIBuffer<T>::~RAIIBuffer()
        {
            if(this->m_buff != nullptr)
            {
                free(this->m_buff);
            }
        }

        template<typename T>
        RAIIBuffer<T>::operator T*() const
        {
            return this->m_buff;
        }
        template<typename T>
        T* RAIIBuffer<T>::getBuff() const
        {
            return this->m_buff;
        }

        template<typename T>
        int RAIIBuffer<T>::allocate(size_t n)
        {
            this->m_buff = reinterpret_cast<T*>(malloc(n * sizeof(T)));
            if(this->m_buff == nullptr)
            {
                printf("Error: could not allocate enough memeory.\n");
                return 1;
            }
            else
            {
                return 0;
            }
        }

        template<typename T>
        int RAIIBuffer<T>::reallocate(size_t n)
        {
            T* prev = this->m_buff;
            this->m_buff = realloc(this->m_buff, n * sizeof(T));
            if(this->m_buff == nullptr)
            {
                printf("Error: could not allocate enough memeory.\n");
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}
