#include "../pch.hpp"
#include "args.hpp"
#include "../common/mdpl_common.hpp"
#include "compile.hpp"
#include "link.hpp"

int main(int argc, char** argv)
{
	//catch all uncaught exceptions and return exit failure so stack unwinding takes place
	try
	{
		//process the cli arguments
		mdpl::args::CLIOptions cliOptions;
		MDPL_RETERR(mdpl::args::passArgs(&cliOptions, argc, argv));
		MDPL_RETERR(mdpl::args::checkArgs(&cliOptions));
		mdpl::common::RAIIBuffer<char> outputName;
		MDPL_RETERR(mdpl::args::defaultArgs(&cliOptions, &outputName));

		if(cliOptions.hasHelp)
		{
			printf("Usage: mdpl [OPTION]...\n");
			printf("Build, compile, and link MDPL code.\n\n");
			cag_option_print(mdpl::args::options, CAG_ARRAY_SIZE(mdpl::args::options), stdout);
		}
		else
		{
			if(cliOptions.isCompilerDebug) { MDPL_RETERR(mdpl::args::debugPrintCLIOptionsStruct(&cliOptions)); }

			if(cliOptions.op == mdpl::args::Operation::Compile)
			{
				mdpl::compiler::compile(&cliOptions);
			}
			else if(cliOptions.op == mdpl::args::Operation::Link)
			{
				mdpl::linker::link(&cliOptions);
			}
			//TODO: implement build mode
		}
	}
	catch(const std::exception&)
	{
		return 1;
	}

	return 0;
}
