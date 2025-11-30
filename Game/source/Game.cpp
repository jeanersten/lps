#include "LPSPCH.hpp"
#include "Game.hpp"
#include "Object2D.hpp"
#include "Object3D.hpp"
#include "Shader.hpp"

namespace LPS
{
  Game::Game()
    : m_running(true)
    , m_window(640, 480, "Little Physics Engine")
    , m_panel(nullptr)
    , m_rect(nullptr)
    , m_block(nullptr)
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
    m_rect = std::make_unique<Object2D>(
      glm::vec2{ -0.5f,  0.5f},
      glm::vec2{  1.0f,  1.0f},
      glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
      "assets/image/Crow.jpg"
    );
    m_block = std::make_unique<Object3D>(
      glm::vec3{ -0.5f,  0.5f, 1.0f},
      glm::vec3{  1.0f,  1.0f, 1.0f },
      glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
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
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    
    static Shader shader{ "assets/shader/vertex_shader.glsl",
                          "assets/shader/fragment_shader.glsl" };

    glm::mat4 model{ 1.0f };
    
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                        glm::vec3(1.0f, 1.0f, 1.0f));  

    
    glm::mat4 view{ 1.0f };

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    glm::mat4 projection{ 1.0f };
    float width { static_cast<float>(m_window.GetWidth()) };
    float height { static_cast<float>(m_window.GetHeight()) };

    projection = glm::perspective(glm::radians(45.0f), width / height,
                                  0.1f, 100.0f);

    shader.Use();
    shader.SetUniformInt("u_Sampler", 0);
    shader.SetUniformMat4f("u_Model", model);
    shader.SetUniformMat4f("u_View", view);
    shader.SetUniformMat4f("u_Projection", projection);
    // m_window.Draw(m_rect.get());
    m_window.Draw(m_block.get());
    glUseProgram(0);
  }
}
