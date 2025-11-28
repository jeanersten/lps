cmake_minimum_required(VERSION 3.30)
project(glad LANGUAGES C)

add_library(glad STATIC
  "source/gl.c"
)

target_include_directories(glad
  PUBLIC
  "include"
)
