#pragma once

#include "Camera.hpp"
#include "Light.hpp"
#include "Object3D.hpp"
#include "Shader.hpp"

namespace LPS
{
  class Ball : public Object3D
  {
  public:
    Ball(const Camera* camera, const Light* light);

    void Draw() override;

  private:
    const Camera* m_camera;
    const Light* m_light;
    Shader m_shader;
  };
}
