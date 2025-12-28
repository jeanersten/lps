#pragma once

#include "Camera.hpp"
#include "DebugPanel.hpp"
#include "Light.hpp"
#include "Object3D.hpp"
#include "Shader.hpp"
#include <vector>

namespace LPS
{
  class Ball : public Object3D
  {
  public:
    Ball(const Camera* camera, const std::vector<Light*>& lights);

    DebugPanel* debug_panel;

    void Draw() override;

  private:
    const Camera* m_camera;
    const std::vector<Light*> m_lights;
    Shader m_shader;
  };
}
