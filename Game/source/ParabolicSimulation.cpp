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
    , drag(0.0f)
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
    float vx0{ ivel * glm::cos(theta) };
    float vy0{ ivel * glm::sin(theta) };
    
    if (drag > 0.0f)
    {
      float exp_drag{ glm::exp(-drag * t) };
      pos.x = ipos.x + (vx0 / drag) * (1.0f - exp_drag);
      pos.y = ipos.y + ((vy0 + grv / drag) / drag) *
              (1.0f - exp_drag) - (grv / drag) * t;
    }
    else
    {
      pos.x = ipos.x + vx0 * t;
      pos.y = ipos.y + vy0 * t - 0.5f * grv * t * t;
    }
  }

  void ParabolicSimulation::Reset()
  {
    if (drag > 0.0f)
    {
      float t_min{ 0.0f };
      float t_max{ 100.0f  };
      
      for (int i = 0; i < 50; ++i)
      {
        float t_mid = (t_min + t_max) * 0.5f;
        
        float theta{ glm::radians(langle) };
        float vy0{ ivel * glm::sin(theta) };
        float exp_drag{ exp(-drag * t_mid) };
        float y{ ipos.y + ((vy0 + grv / drag) / drag) *
                 (1.0f - exp_drag) - (grv / drag) * t_mid };
        
        if (y > 1.0f)
          t_min = t_mid;
        else
          t_max = t_mid;
      }
      
      total_time = (t_min + t_max) * 0.5f;
    }
    else
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
    }

    time = 0.0f;
    pos = ipos;
  }
}
