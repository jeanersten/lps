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
                                float v0, float v1,
                                float v2, float v3) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      glUniform4f(uniform_loc, v0, v1, v2, v3);
    }

  private:
    GLuint m_prog;

    void CheckError(GLuint id, bool is_program) const;
  };
}
