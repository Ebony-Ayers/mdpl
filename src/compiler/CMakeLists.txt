set(compiler_source_files
  main.c
  tokeniser.c
  parser.c
  symbol_table.c
)

add_executable(mdpl_compiler ${compiler_source_files})
set_target_properties(mdpl_compiler PROPERTIES OUTPUT_NAME mdpl)
