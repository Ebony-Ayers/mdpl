set(library_rtlib_source_files
  allocation_tracker.c
  allocator.c
  mdpl_error.c
)
set(tester_rtlib_source_files
  test_runtime_lib.c
)

add_library(mdpl_runtime_lib ${library_rtlib_source_files})
add_executable(mdpl_test_runtime_lib ${tester_rtlib_source_files})
set_target_properties(mdpl_test_runtime_lib PROPERTIES OUTPUT_NAME test_runtime_lib)
