#pragma once

#include "Camera.hpp"
#include "Object2D.hpp"
#include "Shader.hpp"

namespace LPS
{
  class Grid : public Object2D
  {
  public:
    Grid(const Camera* camera);

    void Draw() override;

  private:
    const Camera* m_camera;
    Shader m_shader;
  };
}
