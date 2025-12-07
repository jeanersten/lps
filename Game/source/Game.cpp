#include "LPSPCH.hpp"
#include "Log.hpp"
#include "Game.hpp"
#include "Object2D.hpp"
#include "Object3D.hpp"
#include "Shader.hpp"

namespace LPS
{
  Game::Game()
    : m_running(true)
    , m_window(640, 480, "Little Physics Simulation")
    , m_panel(nullptr)
    , m_rect(nullptr)
    , m_block(nullptr)
    , m_objects()
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
      glm::vec2{ 0.0f, 0.0f }, // Not currently used
      glm::vec2{ 0.5f, 0.5f },
      glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
      "assets/image/Crow.jpg"
    );
    m_block = std::make_unique<Object3D>(
      glm::vec3{ 0.0f, 0.0f, 0.0f }, // Not currently used
      glm::vec3{ 0.5f, 0.5f, 0.5f },
      glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
      "assets/image/Crow.jpg"
    );

    size_t object_count { 10 };

    m_objects.reserve(object_count);

    for (size_t i = 0; i < object_count; i++)
    {
      m_objects.push_back(std::make_unique<Object3D>(
        glm::vec3{ 0.0f, 0.0f, 0.0f }, // Not currently used
        glm::vec3{ 0.5f, 0.5f, 0.5f },
        glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
        "assets/image/Crow.jpg"
      ));
    }
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

    float time_elapsed{ static_cast<float>(glfwGetTime()) };

    static Shader shader{ "assets/shader/vertex_shader.glsl",
                          "assets/shader/fragment_shader.glsl" };

    glm::mat4 projection{ 1.0f };
    float width{ static_cast<float>(m_window.GetWidth()) };
    float height{ static_cast<float>(m_window.GetHeight()) };

    projection = glm::perspective(glm::radians(m_panel->fov), width / height,
                                  0.1f, 100.0f);

    glm::mat4 view{ 1.0f };
    static glm::vec3 cam_pos{ glm::vec3(0.0f, 0.0f,  3.0f) };
    static glm::vec3 cam_target{ glm::vec3(0.0f, 0.0f, -1.0f) };
    static glm::vec3 cam_up { glm::vec3(0.0f, 1.0f,  0.0f) };
    const float cam_speed = 0.05f;

    if (m_window.GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
    {
      cam_pos += cam_speed * cam_target;
    }

    if (m_window.GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
    {
      cam_pos -= cam_speed * cam_target;
    }

    if (m_window.GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
    {
      cam_pos -= glm::normalize(glm::cross(cam_target, cam_up)) * cam_speed;
    }

    if (m_window.GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
    {
      cam_pos += glm::normalize(glm::cross(cam_target, cam_up)) * cam_speed;
    }

    view = glm::lookAt(cam_pos, cam_pos + cam_target, cam_up);

    static glm::vec3 obj_pos[] = {
      glm::vec3{  0.0f,  0.0f,  00.0f },
      glm::vec3{  2.0f,  5.0f, -15.0f },
      glm::vec3{ -1.5f, -2.2f, -02.5f },
      glm::vec3{ -3.8f, -2.0f, -12.3f },
      glm::vec3{  2.4f, -0.4f, -03.5f },
      glm::vec3{ -1.7f,  3.0f, -07.5f },
      glm::vec3{  1.3f, -2.0f, -02.5f },
      glm::vec3{  1.5f,  2.0f, -02.5f },
      glm::vec3{  1.5f,  0.2f, -01.5f },
      glm::vec3{ -1.3f,  1.0f, -01.5f }
    };

    for (int i = 0; i < m_objects.size(); i++)
    {
      glm::mat4 model{ 1.0f };
      float angle{ 20.0f * i };

      model = glm::translate(model, obj_pos[i]);
      model = glm::rotate(model, glm::radians(angle),
                          glm::vec3{ 1.0f, 0.3f, 0.5f });

      shader.Use();
      shader.SetUniformInt("u_Sampler", 0);
      shader.SetUniformMat4f("u_Model", model);
      shader.SetUniformMat4f("u_View", view);
      shader.SetUniformMat4f("u_Projection", projection);

      m_window.Draw(m_objects[i].get());
    }

    glUseProgram(0);
  }
}
