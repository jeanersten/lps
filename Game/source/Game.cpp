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
    , m_delta_time(0.0f)
    , m_camera(nullptr)
    , m_panel(nullptr)
    , m_rect(nullptr)
    , m_block(nullptr)
    , m_objects()
    , m_user_key(false)
    , m_first_mouse(true)
  {
    m_window.SetIcon("assets/icon/lps.png");

    m_window.SetKeyCallback([this](int key, int action) -> void{
      HandleKeyboard(key, action);
    });
    m_window.SetMouseCallback([this](double xpos, double ypos) -> void{
      HandleMouse(xpos, ypos);
    });
    m_window.SetScrollCallback([this](double xoffset, double yoffset) -> void{
      HandleScroll(xoffset, yoffset);
    });

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

    ImGui_ImplGlfw_InitForOpenGL(m_window.GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_ver);

    float wnd_width{ static_cast<float>(m_window.GetWidth()) };
    float wnd_height{ static_cast<float>(m_window.GetHeight()) };

    m_camera = std::make_unique<Camera>(glm::vec2{ wnd_width, wnd_height });

    m_panel = std::make_unique<DebugPanel>();
    m_panel->visibility_callback = [this](bool visible) -> void{
      HandlePanelVisibility(visible);
    };

    m_rect = std::make_unique<Object2D>(
      glm::vec2{ 0.0f, 0.0f }, // Not currently used
      glm::vec2{ 0.5f, 0.5f },
      glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
      "assets/image/Crow.jpg"
    );

    m_block = std::make_unique<Object3D>(
      glm::vec3{ 0.0f, 0.0f, 0.0f }, // Not currently used
      glm::vec3{ 10.0f, 10.0f, 10.0f },
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

      if (glfwGetWindowAttrib(m_window.GetNativeWindow(), GLFW_ICONIFIED) != 0)
      {
        ImGui_ImplGlfw_Sleep(10);

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

      if (m_window.ShouldClose()) m_running = false;
    }
  }

  void Game::Update()
  {
    float cf_time{ static_cast<float>(glfwGetTime()) };
    static float lf_time{ 0.f };

    m_delta_time = cf_time - lf_time;
    lf_time = cf_time;

    float wnd_width{ static_cast<float>(m_window.GetWidth()) };
    float wnd_height{ static_cast<float>(m_window.GetHeight()) };

    m_camera->SetSize(glm::vec2{ wnd_width, wnd_height });
    m_camera->SetFov(m_panel->fov);
    m_camera->Update();

    float cam_speed { m_panel->cam_speed * m_delta_time };

    if (m_user_key[KEY_FORWARD])
    {
      m_camera->MoveForward(cam_speed);
    }

    if (m_user_key[KEY_BACKWARD])
    {
      m_camera->MoveBackward(cam_speed);
    }

    if (m_user_key[KEY_LEFT])
    {
      m_camera->MoveLeft(cam_speed);
    }

    if (m_user_key[KEY_RIGHT])
    {
      m_camera->MoveRight(cam_speed);
    }

    if (m_user_key[KEY_UP])
    {
      m_camera->MoveUp(cam_speed);
    }

    if (m_user_key[KEY_DOWN])
    {
      m_camera->MoveDown(cam_speed);
    }
  }

  void Game::Render()
  {
    m_window.Draw(m_panel.get());

    float time_elapsed{ static_cast<float>(glfwGetTime()) };

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

    static auto light = std::make_unique<Object3D>(
      glm::vec3{ 0.0f, 0.0f, 0.0f }, // Not currently used
      glm::vec3{ 0.25f, 0.25f, 0.25f },
      glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
      ""
    );

    static Shader lgt_shader{ "assets/shader/light_vertex.glsl",
                              "assets/shader/light_fragment.glsl" };

    glm::mat4 lgt_model{ 1.0f };
    static glm::vec3 lgt_pos{ 1.2f, 1.0f, 2.0f };

    lgt_model = glm::mat4(1.0f);
    lgt_model = glm::translate(lgt_model, lgt_pos);

    lgt_shader.Use();

    lgt_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    lgt_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    lgt_shader.SetUniformMat4f("u_Model",lgt_model);

    m_window.Draw(light.get());

    lgt_shader.Unuse();

    static auto object = std::make_unique<Object3D>(
      glm::vec3{ 0.0f, 0.0f, 0.0f }, // Not currently used
      glm::vec3{ 0.5f, 0.5f, 0.5f },
      glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
      "assets/model/Sphere.obj",
      ""
    );

    static Shader obj_shader{ "assets/shader/object_vertex.glsl",
                              "assets/shader/object_fragment.glsl" };

    glm::mat4 obj_model{ 1.0f };

    obj_shader.Use();

    obj_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    obj_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    obj_shader.SetUniformMat4f("u_Model", obj_model);
    obj_shader.SetUniformInt("u_Sampler", 0);
    obj_shader.SetUniformVec3f("u_LightColor", light->GetColor());
    obj_shader.SetUniformVec3f("u_LightPosition", lgt_pos);
    obj_shader.SetUniformVec3f("u_ViewPosition", m_camera->GetPosition());

    m_window.Draw(object.get());

    obj_shader.Unuse();
  }

  void Game::HandleKeyboard(int code, int action)
  {
    if (m_panel->visible)
    {
      for (size_t i = 0; i < KEY_SIZE; i++)
      {
        m_user_key[i] = false;
      }

      return;
    }

    if (action == GLFW_PRESS)
    {
      switch (code)
      {
        case GLFW_KEY_W:
        {
          m_user_key[KEY_FORWARD] = true;
        } break;

        case GLFW_KEY_S:
        {
          m_user_key[KEY_BACKWARD] = true;
        } break;

        case GLFW_KEY_A:
        {
          m_user_key[KEY_LEFT] = true;
        } break;

        case GLFW_KEY_D:
        {
          m_user_key[KEY_RIGHT] = true;
        } break;

        case GLFW_KEY_UP:
        {
          m_user_key[KEY_UP] = true;
        } break;

        case GLFW_KEY_DOWN:
        {
          m_user_key[KEY_DOWN] = true;
        } break;

        default:
        {
          ;
        } break;
      }
    }

    if (action == GLFW_RELEASE)
    {
      switch (code)
      {
        case GLFW_KEY_W:
        {
          m_user_key[KEY_FORWARD] = false;
        } break;

        case GLFW_KEY_S:
        {
          m_user_key[KEY_BACKWARD] = false;
        } break;

        case GLFW_KEY_A:
        {
          m_user_key[KEY_LEFT] = false;
        } break;

        case GLFW_KEY_D:
        {
          m_user_key[KEY_RIGHT] = false;
        } break;

        case GLFW_KEY_UP:
        {
          m_user_key[KEY_UP] = false;
        } break;

        case GLFW_KEY_DOWN:
        {
          m_user_key[KEY_DOWN] = false;
        } break;

        default:
        {
          ;
        } break;
      }
    }
  }

  void Game::HandleMouse(double xpos, double ypos)
  {
    if (m_panel->visible) return;

    static float yaw{ -90.0f };
    static float pitch{  0.0f };
    static float last_mouse_x{ m_window.GetWidth() / 2.0f };
    static float last_mouse_y{ m_window.GetHeight() / 2.0f };
    float mouse_x{ static_cast<float>(xpos) };
    float mouse_y{ static_cast<float>(ypos) };

    if (m_first_mouse)
    {
      last_mouse_x = mouse_x;
      last_mouse_y = mouse_y;
      m_first_mouse = false;
    }

    float xoffset{ mouse_x - last_mouse_x };
    float yoffset{ last_mouse_y - mouse_y };
    last_mouse_x = xpos;
    last_mouse_y = ypos;

    float sensitivity{ 0.1f };
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    m_camera->SetYaw(yaw);
    m_camera->SetPitch(pitch);
  }

  void Game::HandleScroll(double xoffset, double yoffset)
  {
    if (m_panel->visible) return;

    m_panel->fov -= static_cast<float>(yoffset);

    if (m_panel->fov < 1.0f) m_panel->fov = 1.0f;
    if (m_panel->fov > 360.0f) m_panel->fov = 360.0f;
  }

  void Game::HandlePanelVisibility(bool visible)
  {
    glfwSetInputMode(m_window.GetNativeWindow(), GLFW_CURSOR,
                     visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    if (!visible) m_first_mouse = true;
  }
}
