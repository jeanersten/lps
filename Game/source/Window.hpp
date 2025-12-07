#pragma once

#include "Interface.hpp"
#include "Log.hpp"
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <string_view>

namespace LPS
{
  class Window
  {
  public:
    Window(int, int height, std::string_view title);

    ~Window();

    void Draw(IDrawable* object) const;
    void SwapBuffers() const;

    inline int GetWidth() const
    {
      return m_width;
    }

    inline int GetHeight() const
    {
      return m_height;
    }

    inline int GetKeyState(int code) const
    {
      return glfwGetKey(m_object, code);
    }

    inline void* GetNativeWindow() const
    {
      return m_object;
    }

    inline bool ShouldClose() const
    {
      return glfwWindowShouldClose(m_object);
    }

    inline void SetIcon(const std::filesystem::path& path) const
    {
      GLFWimage image{ };

      image.pixels = stbi_load(std::filesystem::absolute(path).string().c_str(),
                               &image.width, &image.height, nullptr, 4);

      LPS_ASSERT(image.pixels != nullptr, "Failed to load window icon");

      glfwSetWindowIcon(m_object, 1, &image);
      stbi_image_free(image.pixels);
    }

  private:
    int m_width;
    int m_height;
    std::string_view m_title;

    GLFWwindow* m_object;
  };
}
