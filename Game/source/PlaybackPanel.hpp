#pragma once

#include "Interface.hpp"
#include <glm/glm.hpp>
#include <functional>

namespace LPS
{
  class PlaybackPanel : public IDrawable
  {
  public:
    PlaybackPanel();

    bool visible;
    float left_padding;

    void Draw() override;

    inline glm::vec2 GetSize() const
    {
      return m_size;
    }

   private:
     glm::vec2 m_size;
  };
}
