set(compiler_source_files
  test_standard_lib.cpp
  string.cpp
)

add_executable(mdpl_test_standard_lib ${compiler_source_files})
set_target_properties(mdpl_test_standard_lib PROPERTIES OUTPUT_NAME test_standard_lib)
