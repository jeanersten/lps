#pragma once

#include "Ball.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace LPS
{
  class ParabolicSimulation
  {
  public:
    ParabolicSimulation();

    float time;
    float total_time;

    glm::vec3 ipos;
    float ivel;
    float langle;

    glm::vec3 pos;
    float grv;

    enum State
    {
      PS_STATE_PLAY,
      PS_STATE_PAUSE,
    };

    inline State GetState() const
    {
      return m_state;
    }

    inline void SetState(State value)
    {
      m_state = value;
    }

    void Update(float delta_time);
    void CalcPosition(float t);
    void Reset();

  private:
    State m_state;
  };
}
