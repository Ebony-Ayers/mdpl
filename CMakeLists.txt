cmake_minimum_required(VERSION 3.15)
if(TARGET mdpl)
    return()
endif()

#set the name of the primary target for future reference. This will only exist in the "scope" of thise file
set(project_name mdpl)

# Set the project name to your project name, "my project" isn't very descriptive as a project can contain multiple distinct targets all grouped under an umbrella
project(${project_name} CXX)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
string(CONCAT project_options_library ${project_name} _project_options)
add_library(${project_options_library} INTERFACE)

#set the c++ standard required
target_compile_features(${project_options_library} INTERFACE cxx_std_20)

#link pthreads for multithreading
option(ENABLE_MULTITHREADING "Adds required linker option for using std::thread" ON)
if(ENABLE_MULTITHREADING)
  find_package(Threads)
  target_link_libraries(${project_options_library} INTERFACE ${CMAKE_THREAD_LIBS_INIT})
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(${project_options_library} INTERFACE -ftime-trace)
  endif()
endif()

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${project_options_library})

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(${project_options_library})

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" ON)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(
    ${project_options_library}
    INTERFACE
    src/pch.hpp)
endif()

add_subdirectory(src/common)
target_link_libraries(mdpl_common PRIVATE ${project_options_library})
add_subdirectory(src/compiler)
target_link_libraries(mdpl_compiler PRIVATE ${project_options_library})
target_link_libraries(mdpl_compiler PUBLIC mdpl_common)
add_subdirectory(vendor/cargs)
target_link_libraries(mdpl_compiler PUBLIC cargs)
add_subdirectory(vendor/libdivide)
target_link_libraries(mdpl_compiler PUBLIC libdivide)

add_subdirectory(src/runtime_lib)
target_link_libraries(mdpl_runtime_lib PRIVATE ${project_options_library})
target_link_libraries(mdpl_runtime_lib PUBLIC libdivide)
target_link_libraries(mdpl_test_runtime_lib PRIVATE ${project_options_library})
target_link_libraries(mdpl_test_runtime_lib PUBLIC mdpl_runtime_lib)
target_link_libraries(mdpl_test_runtime_lib PUBLIC mdpl_common)

add_subdirectory(src/standard_lib)
target_link_libraries(mdpl_test_standard_lib PRIVATE ${project_options_library})
target_link_libraries(mdpl_test_standard_lib PUBLIC mdpl_common)
target_link_libraries(mdpl_test_standard_lib PUBLIC mdpl_runtime_lib)
add_subdirectory(vendor/utf8proc)
target_link_libraries(mdpl_test_standard_lib PUBLIC utf8proc)

#Speeds up compilation
option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
if(ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  set_target_properties(${project_name} PROPERTIES UNITY_BUILD ON)
endif()
