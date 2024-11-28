#include "str_functions.hpp"

namespace mdpl
{
    namespace common
    {
        const char *reverseStrChr(const char *str, int c)
        {
            const char *current = strchr(str, c);
            const char *previous = nullptr;

            while (current != nullptr)
            {
                previous = current;
                current = strchr(current + 1, c);
            }

            return previous;
        }
    }
}