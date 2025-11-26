set(library_compiler_source_files
  allocation_tracker.cpp
  allocator.cpp
  error.cpp
)
set(tester_compiler_source_files
  test_runtime_lib.cpp
)

add_library(mdpl_runtime_lib ${library_compiler_source_files})
add_executable(mdpl_test_runtime_lib ${tester_compiler_source_files})
set_target_properties(mdpl_test_runtime_lib PROPERTIES OUTPUT_NAME test_runtime_lib)
