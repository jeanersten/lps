#pragma once

#include "Interface.hpp"
#include <glm/glm.hpp>
#include <functional>

namespace LPS
{
  class DebugPanel : public IDrawable
  {
    using VisibilityCallbackFunc = std::function<void(bool visible)>;

  public:
    DebugPanel();

    bool visible;
    bool vsync;
    glm::vec3 clear_color;
    float view_z;
    float fov;
    VisibilityCallbackFunc visibility_callback;

    void Draw() override;
  };
}
