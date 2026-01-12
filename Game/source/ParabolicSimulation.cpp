#include "LPSPCH.hpp"
#include "Ball.hpp"
#include "ParabolicSimulation.hpp"

namespace LPS
{
  ParabolicSimulation::ParabolicSimulation()
    : m_state(PS_STATE_PAUSE)
    , time(0.0f)
    , total_time(0.0f)
    , ipos(0.0f, 1.0f, 0.0f)
    , ivel(10.0f)
    , langle(45.0f)
    , pos(ipos)
    , grv(9.8f)
  {
    Reset();
  }

  void ParabolicSimulation::Update(float delta_time)
  {
    if (pos.y <= 0.0f)
    {
      pos.y = 1.0f;
      m_state = PS_STATE_PAUSE;
    }
    if (ipos.y <= 0.0f)
    {
      ipos.y = 1.0f;
      m_state = PS_STATE_PAUSE;
    }

    if (m_state == PS_STATE_PLAY)
    {
      time += delta_time;
      if (time > total_time)
      {
        time = total_time;
        m_state = PS_STATE_PAUSE;
      }

      CalcPosition(time);
    }
  }

  void ParabolicSimulation::CalcPosition(float t)
  {
    float theta{ glm::radians(langle) };
    float vx{ ivel * glm::cos(theta) };
    float vy{ ivel * glm::sin(theta) };
    
    pos.x = ipos.x + vx * t;
    pos.y = ipos.y + vy * t - 0.5f * grv * t * t;
  }

  void ParabolicSimulation::Reset()
  {
    double theta{ glm::radians(langle) };
    double vy0{ ivel * glm::sin(theta) };
    double a{ -0.5f * grv };
    double b{ vy0 };
    double c{ ipos.y - 1.0f };
    double discriminant{  b * b - 4.0f * a * c };
    
    if (discriminant >= 0.0f)
    {
      total_time = (-b - glm::sqrt(discriminant)) / (2.0f * a);
    }
    else
    {
      total_time = 0.0f;
    }

    time = 0.0f;
    pos = ipos;
  }
}
