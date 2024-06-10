#include "../pch.hpp"
#include "args.hpp"
#include "../common/mdpl_common.hpp"

int main(int argc, char** argv)
{
	//catch all uncaught exceptions and return exit failure so stack unwinding takes place
	try
	{
		mdpl::args::CLIOptions cliOptions;
		MDPL_RETERR(mdpl::args::passArgs(&cliOptions, argc, argv));

		if(cliOptions.hasHelp)
		{
			printf("Usage: mdpl [OPTION]...\n");
			printf("Build, compile, and link MDPL code.\n\n");
			cag_option_print(mdpl::args::options, CAG_ARRAY_SIZE(mdpl::args::options), stdout);
		}
		else
			{
			printf("mode = %d\n", cliOptions.op);
			if(cliOptions.outputName != nullptr)
			{
				printf("output name = %s\n", cliOptions.outputName);
			}
			else
			{
				printf("output name = nullptr\n");
			}
			printf("warnings = %ul\n", cliOptions.warnings);
			printf("include dirs = [\n");
			for(size_t i = 0; i < cliOptions.includeDirs.size(); i++)
			{
				printf("\t%s,\n", cliOptions.includeDirs[i]);
			}
			printf("]\n");
			printf("files dirs = [\n");
			for(size_t i = 0; i < cliOptions.files.size(); i++)
			{
				printf("\t%s,\n", cliOptions.files[i]);
			}
			printf("]\n");
			printf("optimisation level = %d\n", cliOptions.optimisationLevel);
		}
	}
	catch(const std::exception&)
	{
		return 1;
	}

	return 0;
}
