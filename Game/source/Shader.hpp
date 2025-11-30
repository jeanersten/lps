#pragma once

#include <GLAD/gl.h>
#include <filesystem>
#include <string_view>

namespace LPS
{
  class Shader
  {
  public:
    Shader(const std::filesystem::path& vshader_path,
           const std::filesystem::path& fshader_path);
    ~Shader();

    void Use() const;

    inline void SetUniformVec4f(std::string_view name,
                                float val0, float val1,
                                float val2, float val3) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      glUniform4f(uniform_loc, val0, val1, val2, val3);
    }

    inline void SetUniformInt(std::string_view name, int val) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };
      
      glUniform1i(uniform_loc, val);
    }

  private:
    GLuint m_prog;

    void CheckError(GLuint id, bool is_program) const;
  };
}
