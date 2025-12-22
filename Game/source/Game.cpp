#include "LPSPCH.hpp"
#include "Ball.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "Grid.hpp"
#include "DebugPanel.hpp"
#include "Light.hpp"
#include "Window.hpp"

namespace LPS
{
  Game::Game()
    : m_running(true)
    , m_window(640, 480, "Little Physics Simulation")
    , m_delta_time(0.0f)
    , m_camera(nullptr)
    , m_panel(nullptr)
    , m_light(nullptr)
    , m_ball(nullptr)
    , m_grid(nullptr)
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
    m_light = std::make_unique<Light>(m_camera.get());
    m_ball = std::make_unique<Ball>(m_camera.get(), m_light.get());
    m_grid = std::make_unique<Grid>(m_camera.get());

    m_panel->visibility_callback = [this](bool visible) -> void {
      HandlePanelVisibility(visible);
    };
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

      glDisable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);
      glClearColor(m_panel->clear_color.r, m_panel->clear_color.g,
                   m_panel->clear_color.b, 1.0f);
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
    m_window.Draw(m_light.get());
    m_window.Draw(m_ball.get());
    m_window.Draw(m_grid.get());

    m_ball->SetDrawFrameMode(m_panel->frame_mode);
    m_grid->SetDrawFrameMode(m_panel->frame_mode);
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
    static float pitch{ -45.0f };
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
