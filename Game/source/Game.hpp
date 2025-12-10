#pragma once

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

    std::unique_ptr<DebugPanel> m_panel;
    std::unique_ptr<Object2D> m_rect;
    std::unique_ptr<Object3D> m_block;

    std::vector<std::unique_ptr<Object3D>> m_objects;

    enum UserKey
    {
      KEY_UP,
      KEY_DOWN,
      KEY_LEFT,
      KEY_RIGHT,
      KEY_SIZE
    };

    bool m_user_key[KEY_SIZE];

    glm::vec3 cam_pos{ glm::vec3(0.0f, 0.0f,  3.0f) };
    glm::vec3 cam_target{ glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 cam_up { glm::vec3(0.0f, 1.0f,  0.0f) };

    void Update();
    void Render();

    void HandleKeyboard(int code, int action);
    void HandleMouse(double xpos, double ypos);
    void HandleScroll(double xoffset, double yoffset);
    void HandlePanelVisibility(bool visible);
  };
}
