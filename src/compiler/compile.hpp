#ifndef MDPL_COMPILE_HEADER_GUARD
#define MDPL_COMPILE_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "args.hpp"

namespace mdpl
{
    namespace compiler
    {
        int compile(mdpl::args::CLIOptions* cliOptions);
    }
}

#endif //MDPL_COMPILE_HEADER_GUARD