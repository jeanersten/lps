#include "LPSPCH.hpp"
#include "Camera.hpp"

namespace LPS
{
  Camera::Camera(glm::vec2 size)
    : m_pos(0.0f, 5.0f, 5.0f)
    , m_size(size)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(-45.0f)
    , m_fov(45.0f)
  {}

  void Camera::Update()
  {
    glm::vec3 front{ 0.0f, 0.0f, 0.0f };

    front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    front.y = glm::sin(glm::radians(m_pitch));
    front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

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
    m_pos -= m_right * speed;
  }

  void Camera::MoveRight(float speed)
  {
    m_pos += m_right * speed;
  }

  void Camera::MoveUp(float speed)
  {
    m_pos += m_up * speed;
  }

  void Camera::MoveDown(float speed)
  {
    m_pos -= m_up * speed;
  }
}
