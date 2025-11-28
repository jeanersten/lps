cmake_minimum_required(VERSION 3.30)
project(glfw LANGUAGES C)

add_library(glfw STATIC
  "source/context.c"
  "source/init.c"
  "source/input.c"
  "source/monitor.c"
  "source/platform.c"
  "source/vulkan.c"
  "source/window.c"
  "source/egl_context.c"
  "source/osmesa_context.c"
  "source/null_init.c"
  "source/null_monitor.c"
  "source/null_window.c"
  "source/null_joystick.c"
)

if(WIN32)
  target_compile_definitions(glfw
    PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _GLFW_WIN32
    _UNICODE
    UNICODE
  )
  target_sources(glfw
    PRIVATE
    "source/win32_init.c"
    "source/win32_joystick.c"
    "source/win32_monitor.c"
    "source/win32_window.c"
    "source/wgl_context.c"
    "source/win32_module.c"
    "source/win32_time.c"
    "source/win32_thread.c"
  )
elseif(APPLE)
  target_compile_definitions(glfw
    PRIVATE
    _GLFW_COCOA
  )
  target_sources(glfw
    PRIVATE
    "source/cocoa_init.m"
    "source/cocoa_joystick.m"
    "source/cocoa_monitor.m"
    "source/cocoa_window.m"
    "source/nsgl_context.m"
    "source/cocoa_time.c"
    "source/posix_module.c"
    "source/posix_thread.c"
  )
else()
  # Add support for X11/Linux
endif()

target_include_directories(glfw
  PRIVATE
  "source"
  PUBLIC
  "include"
)
