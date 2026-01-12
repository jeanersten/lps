#include "LPSPCH.hpp"
#include "Ball.hpp"
#include "ParabolicSimulation.hpp"

namespace LPS
{
  ParabolicSimulation::ParabolicSimulation()
    : m_state(PS_STATE_PAUSE)
    , time(0.0f)
    , total_time(0.0f)
    , ipos(0.0f, 0.0f, 0.0f)
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
    time = 0.0f;
    total_time = (2.0f * ivel * glm::sin(glm::radians(langle))) / grv;
    pos = ipos;
  }
}
