#include "LPSPCH.hpp"
#include "Game.hpp"
#include "Object.hpp"
#include "Shader.hpp"

namespace LPS
{
  Game::Game()
    : m_running(true)
    , m_window(640, 480, "Little Physics Engine")
    , m_panel(nullptr)
    , m_object(nullptr)
  {
    m_window.SetIcon("assets/icon/lps.png");

    const char* glsl_ver{ "#version 330" };
    float main_scale{
      ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor())
    };

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io{ ImGui::GetIO() };

    io.LogFilename = nullptr;
    io.IniFilename = nullptr;

    ImGuiStyle& style{ ImGui::GetStyle() };

    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(
      static_cast<GLFWwindow*>(m_window.GetNativeWindow()), true
    );
    ImGui_ImplOpenGL3_Init(glsl_ver);

    m_panel = std::make_unique<DebugPanel>();
    m_object = std::make_unique<Object>(
      glm::vec2{ -0.5f,  0.5f},
      glm::vec2{  1.0f,  1.0f},
      glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
      "assets/image/Crow.jpg"
    );
  }

  Game::~Game()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void Game::Run()
  {
    while (m_running)
    {
      glfwPollEvents();
      if (glfwGetWindowAttrib(
        static_cast<GLFWwindow*>(m_window.GetNativeWindow()),
        GLFW_ICONIFIED) != 0
      )
      {
        ImGui_ImplGlfw_Sleep(50);

        continue;
      }

      glClearColor(m_panel->clear_color.r, m_panel->clear_color.g,
                   m_panel->clear_color.b, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      Update();
      Render();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      m_window.SwapBuffers();

      ImGui_ImplGlfw_Sleep(m_panel->sleep_count);

      if (m_window.ShouldClose()) m_running = false;
    }
  }

  void Game::Update()
  {}

  void Game::Render()
  {
    m_window.Draw(m_panel.get());
    
    static Shader shader{ "assets/shader/tex2d_vertex.glsl",
                          "assets/shader/tex2d_fragment.glsl" };

    shader.Use();
    shader.SetUniformInt("img_texture", 0);
    m_window.Draw(m_object.get());
    glUseProgram(0);
  }
}
