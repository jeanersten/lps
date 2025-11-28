cmake_minimum_required(VERSION 3.30)
project(imgui LANGUAGES CXX)

add_library(imgui STATIC
  "source/backends/imgui_impl_glfw.cpp"
  "source/backends/imgui_impl_opengl3.cpp"
  "source/imgui.cpp"
  "source/imgui_demo.cpp"
  "source/imgui_draw.cpp"
  "source/imgui_tables.cpp"
  "source/imgui_widgets.cpp"
)

target_include_directories(imgui
  PRIVATE
  "include/ImGUI"
  PUBLIC
  "include"
)

target_link_libraries(imgui
  PRIVATE
  glfw
)
