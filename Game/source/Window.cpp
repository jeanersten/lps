#include "LPSPCH.hpp"
#include "Interface.hpp"
#include "Log.hpp"
#include "Window.hpp"

namespace LPS
{
  Window::Window(int width, int height, std::string_view title)
    : m_width(width)
    , m_height(height)
    , m_title(title)
    , m_object(nullptr)
  {
    glfwSetErrorCallback([](int, const char* description) -> void{
      LPS_LOG_ERROR("{}", description);
    });

    if (!glfwInit())
    {
      LPS_LOG_FATAL("Failed to initialize GLFW!");

      std::exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_object = glfwCreateWindow(m_width, m_height, m_title.data(),
                                nullptr, nullptr);

    if (m_object == nullptr) std::exit(-1);

    glfwMakeContextCurrent(m_object);

    if (!gladLoadGL(glfwGetProcAddress))
    {
      LPS_LOG_FATAL("Failed to initialize OpenGL 3.3 via GLAD!");

      std::exit(-1);
    }

    glfwSetWindowUserPointer(m_object, this);

    glfwSetFramebufferSizeCallback(m_object, [](GLFWwindow* window,
                                   int width, int height) -> void{
      Window* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

      self->m_width = width;
      self->m_height = height;

      glViewport(0, 0, width, height);
    });

    glfwSetKeyCallback(m_object, [](GLFWwindow* window, int key, int scancode,
                                    int action, int mods) -> void{
      Window* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

      if (self->m_key_callback) self->m_key_callback(key, action);
    });

    glfwSetCursorPosCallback(m_object, [](GLFWwindow* window,
                                          double xpos, double ypos) -> void{
      Window* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

      if (self->m_mouse_callback) self->m_mouse_callback(xpos, ypos);
    });

    glfwSetScrollCallback(m_object, [](GLFWwindow* window,
                                       double xoffset, double yoffset) -> void{
      Window* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

      if (self->m_scroll_callback) self->m_scroll_callback(xoffset, yoffset);
    });
  }

  Window::~Window()
  {
    if (m_object != nullptr) glfwDestroyWindow(m_object);
  }

  void Window::Draw(IDrawable* object) const
  {
    if (object != nullptr) object->Draw();
  }

  void Window::SwapBuffers() const
  {
    glfwSwapBuffers(m_object);
  }
}
