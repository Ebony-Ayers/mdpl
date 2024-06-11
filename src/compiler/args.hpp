#ifndef MDPL_ARGS_HEADER_GUARD
#define MDPL_ARGS_HEADER_GUARD

#include "../common/mdpl_common.hpp"
#include <cargs.h>
#include <vector>

namespace mdpl
{
	namespace args
	{
		//what operation is being requrested of the compiler
		enum class Operation
		{
			None,
			Compile,
			Link,
			Build,
		};
		//a bit set of the enabled vectors
		enum class Warnings : u_int64_t
		{
			None = 0x0000000000000000,
			All =  0xFFFFFFFFFFFFFFFF,
		};
		enum class OptimisationLevel
		{
			None,
			O1,
			O2,
			O3,
			Small
		};
		//a structure representing the command line arguments
		struct CLIOptions
		{
			Operation op;
			const char* outputName;
			Warnings warnings;
			std::vector<const char*> includeDirs;
			std::vector<const char*> files;
			OptimisationLevel optimisationLevel;
			bool hasHelp;
		};

		//cli argument specification as per cargs documentation
		static struct cag_option options[] = {
			{
				.identifier = 'c',
				.access_letters = "c",
				.access_name = "compile",
				.value_name = nullptr,
				.description = "Compile a given source file to an object file. The linker will not be invoked"
			},
			{
				.identifier = 'l',
				.access_letters = "l",
				.access_name = "link",
				.value_name = nullptr,
				.description = "Link object files together. The compiler will not be invoked"
			},
			{
				.identifier = 'b',
				.access_letters = "b",
				.access_name = "build",
				.value_name = nullptr,
				.description = "Build a project"
			},
			{
				.identifier = 'o',
				.access_letters = "o",
				.access_name = "output",
				.value_name = "NAME",
				.description = "The name of the output when compiling or linking"
			},
			{
				.identifier = 'W',
				.access_letters = nullptr,
				.access_name = "no_warn",
				.value_name = "WARNING",
				.description = "Ommit a given warning"
			},
			{
				.identifier = 'I',
				.access_letters = "I",
				.access_name = "include",
				.value_name = "DIR",
				.description = "Add a directory to the include path"
			},
			{
				.identifier = 'O',
				.access_letters = "O",
				.access_name = nullptr,
				.value_name = "LEVEL",
				.description = "Optimisation level. Options are 1, 2, 3, and small"
			},
			{
				.identifier = 'h',
				.access_letters = "h",
				.access_name = "help",
				.value_name = nullptr,
				.description = "Shows the command help"}
		};

		//function to pass the argus
		int passArgs(CLIOptions* cliOptions, const int& argc, char** argv);
		//function to check for invalid arguments
		int checkArgs(CLIOptions* cliOptions);
		//function to fill in any missing arguments with default ones
		int defaultArgs(CLIOptions* cliOptions, RAIIBuffer<char>* outputName);

		//internal function to handle error checking with reguard to setting the mode
		int setMode(CLIOptions* cliOptions, const Operation& mode);
	}
}

#endif //MDPL_ARGS_HEADER_GUARD