#pragma once

namespace LPS
{
  class IDrawable
  {
  public:
    virtual ~IDrawable() = default;

    virtual void Draw() = 0;
  };
}
