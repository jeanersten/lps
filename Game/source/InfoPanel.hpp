#pragma once

#include "Interface.hpp"
#include <glm/glm.hpp>
#include <functional>

namespace LPS
{
  class InfoPanel : public IDrawable
  {
  public:
    InfoPanel();

    bool visible;
    float running_time;
    float fps;
    glm::vec3 view_pos;

    void Draw() override;

    inline glm::vec2 GetSize() const
    {
      return m_size;
    }

  private:
    glm::vec2 m_size;
  };
}
