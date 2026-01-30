set(tester_stdlib_source_files
  test_standard_lib.c
  mdpl_string.c
)

add_executable(mdpl_test_standard_lib ${tester_stdlib_source_files})
set_target_properties(mdpl_test_standard_lib PROPERTIES OUTPUT_NAME test_standard_lib)
