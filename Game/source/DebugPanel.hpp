#pragma once

#include "Interface.hpp"
#include <GLM/glm.hpp>

namespace LPS
{
  class DebugPanel : public IDrawable
  {
  public:
    DebugPanel();

    void Draw() override;

    int sleep_count;
    glm::vec3 clear_color;
  };
}
