#pragma once

#include "DebugPanel.hpp"
#include "Object.hpp"
#include "Window.hpp"
#include <memory>

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
    std::unique_ptr<Object> m_object;

    void Update();
    void Render();
  };
}
