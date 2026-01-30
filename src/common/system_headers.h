#ifndef SYSTEM_HEADERS_HEADER_GUARD
#define SYSTEM_HEADERS_HEADER_GUARD

#define MDPL_PLATFORM_LINUX 1
#define MDPL_PLATFORM_WINDOWS 2
#define MDPL_PLATFORM_MAC 3
#define MDPL_PLATFORM_UNKNOWN 255
#if defined(__linux__)
	#define MDPL_PLATFORM MDPL_PLATFORM_LINUX
#elif defined(__MINGW32__) || defined(__MINGW64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
	#define MDPL_PLATFORM MDPL_PLATFORM_WINDOWS
#elif defined(__APPLE__)
	#define MDPL_PLATFORM MDPL_PLATFORM_MAC
#else
	#define MDPL_PLATFORM MDPL_PLATFORM_UNKNOWN
#endif

#define MDPL_COMPILER_CLANG 1
#define MDPL_COMPILER_GCC 2
#define MDPL_COMPILER_MSVC 3
#define MDPL_COMPILER_INTEL 4
#define MDPL_COMPILER_UNKNOWN 255
#if defined(__clang__)
	#define MDPL_COMPILER "clang"
#elif defined(__GNUC__) || defined(__GNUG__)
	#define MDPL_COMPILER "gcc"
#elif defined(_MSC_VER)
	#define MDPL_COMPILER "msvc"
#elif defined(__INTEL_LLVM_COMPILER)
	#define MDPL_COMPILER "intel"
#else
	#define MDPL_COMPILER "unknown"
#endif


//define access() for non unix platforms
#if MDPL_PLATFORM == MDPL_PLATFORM_LINUX
	#include <unistd.h>
#elif MDPL_PLATFORM == MDPL_PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
	#define R_OK 4
	#define W_OK 2
	#define access(name, mode) _access(name, mode)
#elif MDPL_PLATFORM == MDPL_PLATFORM_MAC
	#include <unistd.h>
#endif

#endif //SYSTEM_HEADERS_HEADER_GUARD