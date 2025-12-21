cmake_minimum_required(VERSION 3.30)
project(LPS)

add_subdirectory("vendor/glad-2.0.8")
add_subdirectory("vendor/glfw-3.4.0")
add_subdirectory("vendor/glm-1.0.2")
add_subdirectory("vendor/imgui-1.92.5")
add_subdirectory("vendor/stbimage-2.30.0")

add_executable(Game
  "source/Ball.cpp"
  "source/Ball.hpp"
  "source/Camera.cpp"
  "source/Camera.hpp"
  "source/DebugPanel.cpp"
  "source/DebugPanel.hpp"
  "source/Game.cpp"
  "source/Game.hpp"
  "source/Interface.hpp"
  "source/Light.cpp"
  "source/Light.hpp"
  "source/Log.cpp"
  "source/Log.hpp"
  "source/Main.cpp"
  "source/Object2D.cpp"
  "source/Object2D.hpp"
  "source/Object3D.cpp"
  "source/Object3D.hpp"
  "source/Shader.cpp"
  "source/Shader.hpp"
  "source/Window.cpp"
  "source/Window.hpp"
)

add_custom_command(
  TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
  "${CMAKE_CURRENT_SOURCE_DIR}/assets"
  "${CMAKE_BINARY_DIR}/$<CONFIG>/bin/assets"
)

target_link_libraries(Game
  PRIVATE
  glad glfw glm imgui stbimage
)

target_compile_definitions(Game
  PRIVATE
  "$<$<CONFIG:Debug>:LPS_DEBUG>"
)

target_precompile_headers(Game
  PRIVATE
  "source/LPSPCH.hpp"
)

set_target_properties(Game
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED TRUE
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin"
)
