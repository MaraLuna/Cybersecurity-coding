cmake_minimum_required(VERSION 3.20)
project(C_Project C)

set(CMAKE_C_STANDARD 17)

# Add path for custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  )

include(CompilerSetup)

# comment

add_executable(executable
  main.c
  picture.c
  picture.h
  arrays.c
  arrays.h
  rules.c
  rules.h
  )

enable_c_warnings_for(executable)
link_math_library(executable)

