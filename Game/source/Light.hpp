#pragma once

#include "Camera.hpp"
#include "Object3D.hpp"
#include "Shader.hpp"

namespace LPS
{
  class Light : public Object3D
  {
  public:
    Light(const Camera* camera);

    void Draw() override;

  private:
    const Camera* m_camera;
    Shader m_shader;
  };
}
