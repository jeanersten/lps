#pragma once

#include "Camera.hpp"
#include "DebugPanel.hpp"
#include "Object2D.hpp"
#include "Object3D.hpp"
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

    float m_delta_time;

    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<DebugPanel> m_panel;

    std::unique_ptr<Object2D> m_rect;
    std::unique_ptr<Object3D> m_block;

    std::vector<std::unique_ptr<Object3D>> m_objects;

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
