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

    std::unique_ptr<DebugPanel> m_panel;
    std::unique_ptr<Object2D> m_rect;
    std::unique_ptr<Object3D> m_block;

    std::vector<std::unique_ptr<Object3D>> m_objects;

    void Update();
    void Render();
  };
}
