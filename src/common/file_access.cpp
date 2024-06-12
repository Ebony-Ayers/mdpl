#include "file_access.hpp"

namespace mdpl
{
	namespace common
	{
		//check that the file exists and the user has the required permissions then open it
		RAIIFile::RAIIFile(const char* filename, const char* mode)
		: m_file(nullptr)
		{
			if(access(filename, F_OK) != 0)
			{
				printf("Error: the file %s does not exist.\n", filename);
				throw std::exception();
			}
			else
			{
				if(strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0)
				{
					if(access(filename, R_OK) != 0)
					{
						printf("Error: you do not have read access to %s.\n", filename);
						throw std::exception();
					}
				}
				else
				{
					if(strcmp(mode, "w") == 0 || strcmp(mode, "w+") == 0 || strcmp(mode, "a") == 0 || strcmp(mode, "a+") == 0)
					{
						if(access(filename, W_OK) != 0)
						{
							printf("Error: you do not have write access to %s.\n", filename);
							throw std::exception();
						}
					}
					else
					{
						this->m_file = fopen(filename, mode);
					}
				}
			}
		}
		//close the file
		RAIIFile::~RAIIFile()
		{
			if(this->m_file != nullptr)
			{
				fclose(this->m_file);
			}
		}

		//implicit cast
		RAIIFile::operator FILE*() const
		{
			return this->m_file;
		}
	}
}