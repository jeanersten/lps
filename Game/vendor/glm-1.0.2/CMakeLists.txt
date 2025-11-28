cmake_minimum_required(VERSION 3.30)
project(glm LANGUAGES CXX)

add_library(glm STATIC
  "source/glm.cpp"
)

target_include_directories(glm
  PUBLIC
  "include"
)
