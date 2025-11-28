cmake_minimum_required(VERSION 3.30)
project(stbimage LANGUAGES CXX)

add_library(stbimage STATIC
  "source/stb_image.cpp"
)

target_include_directories(stbimage
  PUBLIC
  "include"
)
