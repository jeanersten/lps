#pragma once

#include "Shader.hpp"
#include "Window.hpp"
#include <GLAD/gl.h>
#include <glm/glm.hpp>
#include <string_view>

namespace LPS
{
  class Camera
  {
  public:
    Camera(glm::vec2 size);

    void Update();
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);
    void MoveUp(float speed);
    void MoveDown(float speed);

    inline glm::mat4 GetViewMatrix() const
    {
      return glm::lookAt(m_pos, m_pos + m_front, m_up);
    }

    inline glm::mat4 GetProjectionMatrix() const
    {
      return glm::perspective(glm::radians(m_fov), m_size.x / m_size.y,
                              0.1f, 1000.0f);
    }

    inline glm::vec3 GetPosition() const
    {
      return m_pos;
    }

    inline float GetYaw() const
    {
      return m_yaw;
    }

    inline float GetPitch() const
    {
      return m_pitch;
    }

    inline float GetFov() const
    {
      return m_fov;
    }

    inline void SetPosition(glm::vec3 val)
    {
      m_pos = val;
    }

    inline void SetSize(glm::vec2 val)
    {
      m_size = val;
    }

    inline void SetYaw(float val)
    {
      m_yaw = val;
    }

    inline void SetPitch(float val)
    {
      m_pitch = val;
    }

    inline void SetFov(float val)
    {
      m_fov = val;
    }

  private:
    glm::vec3 m_pos;
    glm::vec2 m_size;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    float m_yaw;
    float m_pitch;
    float m_fov;
  };
}
