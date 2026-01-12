#include "LPSPCH.hpp"
#include "Ball.hpp"
#include "Camera.hpp"
#include "DebugPanel.hpp"
#include "Game.hpp"
#include "Grid.hpp"
#include "InfoPanel.hpp"
#include "Light.hpp"
#include "ParabolicSimulation.hpp"
#include "PlaybackPanel.hpp"

namespace LPS
{
  Game::Game()
    : m_running(true)
    , m_window(640, 480, "Little Physics Simulation")
    , m_time(0.0f)
    , m_delta_time(0.0f)
    , m_simulation(nullptr)
    , m_camera(nullptr)
    , m_debug_panel(nullptr)
    , m_playback_panel(nullptr)
    , m_llight(nullptr)
    , m_rlight(nullptr)
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

    m_simulation = std::make_unique<ParabolicSimulation>();

    float wnd_width{ static_cast<float>(m_window.GetWidth()) };
    float wnd_height{ static_cast<float>(m_window.GetHeight()) };

    m_camera = std::make_unique<Camera>(glm::vec2{ wnd_width, wnd_height });
    m_info_panel = std::make_unique<InfoPanel>();
    m_debug_panel = std::make_unique<DebugPanel>();
    m_playback_panel = std::make_unique<PlaybackPanel>();
    m_llight = std::make_unique<Light>(m_camera.get());
    m_rlight = std::make_unique<Light>(m_camera.get());
    m_ball = std::make_unique<Ball>(
      m_camera.get(), std::vector<Light*>{ m_llight.get(), m_rlight.get() }
    );
    m_grid = std::make_unique<Grid>(m_camera.get());

    m_window.Maximize();

    m_debug_panel->cam = m_camera.get();
    m_debug_panel->visibility_callback = [this](bool visible) -> void {
      HandlePanelVisibility(visible);
    };

    m_playback_panel->simulation = m_simulation.get();

    m_ball->debug_panel = m_debug_panel.get();
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
      glEnable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
      glClearColor(m_debug_panel->clear_color.r, m_debug_panel->clear_color.g,
                   m_debug_panel->clear_color.b, 1.0f);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    m_time = static_cast<float>(glfwGetTime());

    float cf_time{ m_time };
    static float lf_time{ 0.f };

    m_delta_time = cf_time - lf_time;
    lf_time = cf_time;

    m_info_panel->running_time = m_time;
    m_info_panel->view_pos = m_camera->GetPosition();
    m_info_panel->ball_pos = m_ball->GetPosition();
    m_playback_panel->left_padding = m_debug_panel->GetSize().x;

    float wnd_width{ static_cast<float>(m_window.GetWidth()) };
    float wnd_height{ static_cast<float>(m_window.GetHeight()) };

    m_camera->SetSize(glm::vec2{ wnd_width, wnd_height });
    m_camera->SetFov(m_debug_panel->cam_fov);
    m_camera->Update();

    float cam_speed { m_debug_panel->cam_speed * m_delta_time };

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

    glm::vec3 lrot{
      m_ball->GetPosition().x + glm::cos(m_time) *
        m_debug_panel->rot_light_dist,
      m_ball->GetPosition().y,
      m_ball->GetPosition().z + glm::sin(m_time) *
        m_debug_panel->rot_light_dist
    };

    glm::vec3 rrot{
      m_ball->GetPosition().x - glm::cos(m_time) *
        m_debug_panel->rot_light_dist,
      m_ball->GetPosition().y,
      m_ball->GetPosition().z - glm::sin(m_time) *
        m_debug_panel->rot_light_dist
    };

    m_llight->SetPosition(glm::vec3{ lrot });
    m_rlight->SetPosition(glm::vec3{ rrot });

    m_simulation->Update(m_delta_time);

    m_ball->SetPosition(m_simulation->pos);
  }

  void Game::Render()
  {
    m_window.Draw(m_info_panel.get());
    m_window.Draw(m_debug_panel.get());
    m_window.Draw(m_playback_panel.get());
    m_window.Draw(m_llight.get());
    m_window.Draw(m_rlight.get());
    m_window.Draw(m_ball.get());
    m_window.Draw(m_grid.get());

    m_llight->SetDrawFrameMode(m_debug_panel->frame_mode);
    m_rlight->SetDrawFrameMode(m_debug_panel->frame_mode);
    m_ball->SetDrawFrameMode(m_debug_panel->frame_mode);
    m_grid->SetDrawFrameMode(m_debug_panel->frame_mode);
  }

  void Game::HandleKeyboard(int code, int action)
  {
    if (m_debug_panel->visible)
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
    if (m_debug_panel->visible) return;

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
    if (m_debug_panel->visible) return;

    m_debug_panel->cam_fov -= static_cast<float>(yoffset);

    if (m_debug_panel->cam_fov < 1.0f) m_debug_panel->cam_fov = 1.0f;
    if (m_debug_panel->cam_fov > 360.0f) m_debug_panel->cam_fov = 360.0f;
  }

  void Game::HandlePanelVisibility(bool visible)
  {
    glfwSetInputMode(m_window.GetNativeWindow(), GLFW_CURSOR,
                     visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    if (!visible) m_first_mouse = true;
  }
}
