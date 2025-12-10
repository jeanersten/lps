#pragma once

#include "Shader.hpp"
#include "Window.hpp"
#include <GLAD/gl.h>
#include <string_view>

namespace LPS
{
  class Camera
  {
  public:
    Camera();

    void Update();
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);

    inline glm::mat4 GetMatrix() const
    {
      return glm::lookAt(m_pos, m_pos + m_front, m_up);
    }

    inline float GetFov() const
    {
      return m_fov;
    }

    inline void SetFov(float val)
    {
      m_fov = val;
    }

    inline void SetYaw(float val)
    {
      m_yaw = val;
    }

    inline void SetPitch(float val)
    {
      m_pitch = val;
    }

  private:
    glm::vec3 m_pos;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    float m_fov;
    float m_yaw;
    float m_pitch;
  };
}
