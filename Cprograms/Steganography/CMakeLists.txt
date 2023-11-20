cmake_minimum_required(VERSION 3.20)
project(C_Project C)

set(CMAKE_C_STANDARD 17)

# Add path for custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  )

include(CompilerSetup)

add_executable(steganography
  main.c
  conv.c conv.h
  ppm.c ppm.h
  steganography.c steganography.h
  read_bin.c read_bin.h

  )

enable_c_warnings_for(steganography)
link_math_library(steganography)
