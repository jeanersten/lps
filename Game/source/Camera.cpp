#include "LPSPCH.hpp"
#include "Camera.hpp"

namespace LPS
{
  Camera::Camera()
    : m_pos(0.0f, 0.0f, 3.0f)
    , m_front(0.0f, 0.0f, 3.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_right(0.0f, 0.0f, 0.0f)
    , m_fov(45.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
  {}

  void Camera::Update()
  {
    glm::vec3 front{ 0.0f, 0.0f, 0.0f };

    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, { 0.0f, 1.0f, 0.0f }));
    m_up = glm::normalize(glm::cross(m_right, m_front));
  }

  void Camera::MoveForward(float speed)
  {
    m_pos += m_front * speed;
  }
  void Camera::MoveBackward(float speed)
  {
    m_pos -= m_front * speed;
  }
  void Camera::MoveLeft(float speed)
  {
    m_pos -= glm::normalize(glm::cross(m_front, m_up)) * speed;
  }
  void Camera::MoveRight(float speed)
  {
    m_pos += glm::normalize(glm::cross(m_front, m_up)) * speed;
  }
}
