#include "args.hpp"

namespace mdpl
{
	namespace args
	{
		int passArgs(CLIOptions* cliOptions, const int& argc, char** argv)
		{
			//default values for options struct
			cliOptions->op = Operation::None;
			cliOptions->outputName = nullptr;
			cliOptions->warnings = Warnings::All;
			cliOptions->includeDirs = std::vector<const char*>();
			cliOptions->files = std::vector<const char*>();
			cliOptions->optimisationLevel = OptimisationLevel::None;
			cliOptions->hasHelp = false;
			cliOptions->isCompilerDebug = false;

			//run cargs as described on the github
			//this loop will get any arguments starting with a -
			cag_option_context context;
			cag_option_init(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
			while (cag_option_fetch(&context)) {
				switch (cag_option_get_identifier(&context)) {
					case 'c':
						{ MDPL_RETERR(setMode(cliOptions, Operation::Compile)); }
						break;
					case 'l':
						{ MDPL_RETERR(setMode(cliOptions, Operation::Link)); }
						break;
					case 'b':
						{ MDPL_RETERR(setMode(cliOptions, Operation::Build)); }
						break;
					case 'o':
						{
							if(cliOptions->outputName != nullptr)
							{
								printf("Argument error: cannot set output name twice.\n");
								return 1;
							}
							else
							{
								cliOptions->outputName = cag_option_get_value(&context);
							}
						}
						break;
					case 'W':
						{
							printf("Warning: errors are not yet implemented.\n");
						}
						break;
					case 'I':
						{
							printf("Warning: include dirs are not yet implemented.\n");
							const char* dir = cag_option_get_value(&context);
							
							//check if the include dir is already in the list and if it has not add it to the list
							for(size_t i = 0; i < cliOptions->includeDirs.size(); i++)
							{
								if(strcmp(dir, cliOptions->files[i]) == 0)
								{
									printf("Argument error: include dir %s had already been included.\n", dir);
									return 1;
								}
							}
							//if we get to this point in the code then we must not have already included the dir
							cliOptions->includeDirs.push_back(dir);
						}
						break;
					case 'O':
						{
							if(cliOptions->optimisationLevel != OptimisationLevel::None)
							{
								printf("Argument error: cannot specify optimisation level twice.\n");
								return 1;
							}
							const char* level = cag_option_get_value(&context);
							if(level != nullptr)
							{
								if(strcmp(level, "1") == 0)
								{
									cliOptions->optimisationLevel = OptimisationLevel::O1;
								}
								else if(strcmp(level, "2") == 0)
								{
									cliOptions->optimisationLevel = OptimisationLevel::O2;
								}
								else if(strcmp(level, "3") == 0)
								{
									cliOptions->optimisationLevel = OptimisationLevel::O3;
								}
								else if(strcmp(level, "small") == 0)
								{
									cliOptions->optimisationLevel = OptimisationLevel::Small;
								}
								else
								{
									printf("Argument error: optimisation level %s is unrecognised.\n", level);
									return 1;
								}
							}
							else
							{
								printf("Argument error: no optimisation level specified for argument -O.\n");
								return 1;
							}
							break;
						}
					case 'h':
						{
							cliOptions->hasHelp = true;
						}
						break;
					case '~':
						{
							cliOptions->isCompilerDebug = true;
						}
					case '?':
						{ cag_option_print_error(&context, stdout); }
						break;
				}
			}

			//this loop is for any arguments passed that do not start with a -
			for (int param_index = cag_option_get_index(&context); param_index < argc; ++param_index)
			{
				//if the given file has not been already listed store it
				for(size_t i = 0; i < cliOptions->files.size(); i++)
				{
					if(strcmp(argv[param_index], cliOptions->files[i]) == 0)
					{
						printf("Argument error: file %s had already been specified.\n", argv[param_index]);
						return 1;
					}
				}
				//if we have reached here then we must have a file that has not been listed before
				cliOptions->files.push_back(argv[param_index]);
			}

			return 0;
		}

		int checkArgs(CLIOptions* cliOptions)
		{
			if((cliOptions->op == Operation::Compile) || (cliOptions->op == Operation::Link))
			{
				if(cliOptions->files.size() == 0)
				{
					printf("Argument error: must specify at least one file to build or link.\n");
					return 1;
				}
			}
			if(cliOptions->op == Operation::Build)
			{
				if(cliOptions->outputName != nullptr)
				{
					printf("Argument error: cannot speficy an output when building a project.\n");
					return 1;
				}
				if(cliOptions->files.size() > 1)
				{
					printf("Argument error: you can only specify one project at a time to build.\n");
					return 1;
				}
			}
			return 0;
		}

		int defaultArgs(CLIOptions* cliOptions, common::RAIIBuffer<char>* outputName)
		{
			if(cliOptions->op == Operation::None)
			{
				cliOptions->op = Operation::Build;
			}
			if(cliOptions->outputName == nullptr)
			{
				if((cliOptions->op == Operation::Compile) || (cliOptions->op == Operation::Link))
				{
					//we do not need to check if the the size of files is non zero as if that is true an error would already have been triggerd
					
					//if no output file has been specified we will replace the extention of the input
					const char* fileExtension = common::reverseStrChr(cliOptions->files[0], '.');
					const size_t fileNameLength = fileExtension - cliOptions->files[0];
					//outputName is currently a unallocated pointer so we need to alocate it
					if(cliOptions->op == Operation::Compile) { MDPL_RETERR(outputName->allocate(fileNameLength + 3)); }
					else                                     { MDPL_RETERR(outputName->allocate(fileNameLength)); }
					//copy the file name sans the extension
					memcpy(outputName->getBuff(), cliOptions->files[0], fileNameLength);
					//copy the extension and the null terminating character
					if(cliOptions->op == Operation::Compile) { memcpy(outputName->getBuff() + fileNameLength, ".o", 3); }
					else                                     { *(outputName->getBuff() + fileNameLength) = '\0'; }
					//set the output name in the struct
					cliOptions->outputName = outputName->getBuff();
				}
			}
			if(cliOptions->optimisationLevel == OptimisationLevel::None)
			{
				cliOptions->optimisationLevel = OptimisationLevel::O1;
			}
			return 0;
		}

		int debugPrintCLIOptionsStruct(CLIOptions* cliOptions)
		{
			printf("cliOptions = \n{\n");
			printf("\tmode = %d\n", cliOptions->op);
			if(cliOptions->outputName != nullptr)
			{
				printf("\toutput name = %s\n", cliOptions->outputName);
			}
			else
			{
				printf("\toutput name = nullptr\n");
			}
			printf("\twarnings = %lu\n", cliOptions->warnings);
			printf("\tinclude dirs = [\n");
			for(size_t i = 0; i < cliOptions->includeDirs.size(); i++)
			{
				printf("\t\t%s,\n", cliOptions->includeDirs[i]);
			}
			printf("\t]\n");
			printf("\tfiles dirs = [\n");
			for(size_t i = 0; i < cliOptions->files.size(); i++)
			{
				printf("\t\t%s,\n", cliOptions->files[i]);
			}
			printf("\t]\n");
			printf("\toptimisation level = %d\n", cliOptions->optimisationLevel);
			printf("}\n");
			return 0;
		}

		//check if the mode has not been set before and if not set it
		int setMode(CLIOptions* cliOptions, const Operation& mode)
		{
			if(cliOptions->op != Operation::None)
			{
				printf("Argument error: cannot specify ");
				if(cliOptions->op == Operation::Compile)
				{
					printf("compile");
				}
				else if(cliOptions->op == Operation::Link)
				{
					printf("link");
				}
				else
				{
					printf("build");
				}
				printf(" mode after already specifying ");
				if(cliOptions->op == Operation::Compile)
				{
					printf("compile");
				}
				else if(cliOptions->op == Operation::Link)
				{
					printf("link");
				}
				else
				{
					printf("build");
				}
				printf(" mode.\n");
			}
			else
			{
				cliOptions->op = mode;
			}
			return 0;
		}
	}
}