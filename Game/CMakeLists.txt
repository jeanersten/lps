cmake_minimum_required(VERSION 3.30)
project(LPS)

add_subdirectory("vendor/glad-2.0.8")
add_subdirectory("vendor/glfw-3.4.0")
add_subdirectory("vendor/glm-1.0.2")
add_subdirectory("vendor/imgui-1.92.5")
add_subdirectory("vendor/stbimage-2.30.0")

add_executable(Game
  "source/Main.cpp"
)

target_link_libraries(Game
  PRIVATE
  glad glfw glm imgui stbimage
)

target_precompile_headers(Game
  PRIVATE
  "source/LPSPCH.hpp"
)

set_target_properties(Game
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED TRUE
)
