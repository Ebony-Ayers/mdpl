#ifndef FILE_ACCESS_HEADER_GUARD
#define FILE_ACCESS_HEADER_GUARD

#include "../pch.hpp"
#include "mdpl_common.hpp"

namespace mdpl
{
	namespace common
	{
		//simple RAII file which will implicity convert to FILE* for c functions
		class RAIIFile
		{
			public:
				//mode has the same options as fopen()
				RAIIFile(const char* filename, const char* mode);
				~RAIIFile();

				operator FILE*() const;
				FILE* getFile() const;
			
			private:
				FILE* m_file;
		};
	}
}

#endif //FILE_ACCESS_HEADER_GUARD