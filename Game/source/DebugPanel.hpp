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

    bool visible;
    int sleep_count;
    glm::vec3 clear_color;
    float view_z;
    float fov;
    float f1;
    float f2;
    float f3;
  };
}
