#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "allocator.hpp"

int main(int argc, char** argv)
{
    mdpl::runtimeLib::allocationTracker::constructor(&mdpl::runtimeLib::allocationTracker::globalAllocationTracker, 256);

    mdpl::runtimeLib::allocationTracker::destructor(&mdpl::runtimeLib::allocationTracker::globalAllocationTracker);

    return 0;
}