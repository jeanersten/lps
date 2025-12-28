#pragma once

#include "Camera.hpp"
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
    bool frame_mode;
    glm::vec3 clear_color;
    Camera* cam;
    float cam_fov;
    float cam_speed;
    bool lighting;
    float light_linear;
    float light_quadratic;
    float rot_light_dist;
    VisibilityCallbackFunc visibility_callback;

    void Draw() override;

    inline glm::vec2 GetSize() const
    {
      return m_size;
    }

  private:
    glm::vec2 m_size;
  };
}
