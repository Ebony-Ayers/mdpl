set(project_source_files
  main.cpp
)

add_executable(mdpl_compiler ${project_source_files})
set_target_properties(mdpl_compiler PROPERTIES OUTPUT_NAME mdpl)
