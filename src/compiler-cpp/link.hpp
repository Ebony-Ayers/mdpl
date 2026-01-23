#ifndef MDPL_LINK_HEARDER_GUARD
#define MDPL_LINK_HEARDER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "args.hpp"

namespace mdpl
{
    namespace linker
    {
        int link(mdpl::args::CLIOptions* cliOptions);
    }
}

#endif //MDPL_LINK_HEARDER_GUARD