#pragma once

#include "Ball.hpp"
#include "Camera.hpp"
#include "DebugPanel.hpp"
#include "Grid.hpp"
#include "InfoPanel.hpp"
#include "Light.hpp"
#include "PlaybackPanel.hpp"
#include "Window.hpp"
#include <memory>
#include <vector>

namespace LPS
{
  class Game
  {
  public:
    Game();

    ~Game();

    void Run();

  private:
    bool m_running;
    Window m_window;

    float m_time;
    float m_delta_time;

    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<InfoPanel> m_info_panel;
    std::unique_ptr<DebugPanel> m_debug_panel;
    std::unique_ptr<PlaybackPanel> m_playback_panel;
    std::unique_ptr<Light> m_llight;
    std::unique_ptr<Light> m_rlight;
    std::unique_ptr<Ball> m_ball;
    std::unique_ptr<Grid> m_grid;

    enum UserKey
    {
      KEY_FORWARD,
      KEY_BACKWARD,
      KEY_LEFT,
      KEY_RIGHT,
      KEY_UP,
      KEY_DOWN,
      KEY_SIZE
    };

    bool m_user_key[KEY_SIZE];
    bool m_first_mouse;

    void Update();
    void Render();

    void HandleKeyboard(int code, int action);
    void HandleMouse(double xpos, double ypos);
    void HandleScroll(double xoffset, double yoffset);
    void HandlePanelVisibility(bool visible);
  };
}
