set(compiler_source_files
  allocation_tracker.cpp
  allocator.cpp
  test_runtime_lib.cpp
)

add_executable(mdpl_test_runtime_lib ${compiler_source_files})
set_target_properties(mdpl_test_runtime_lib PROPERTIES OUTPUT_NAME test_runtime_lib)
