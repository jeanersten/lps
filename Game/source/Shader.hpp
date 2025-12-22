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
    void Unuse() const;

    inline void SetUniformVec3f(std::string_view name,
                                float val0, float val1, float val2) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      CheckUniformExistence(uniform_loc, name);

      glUniform3f(uniform_loc, val0, val1, val2);
    }

    inline void SetUniformVec3f(std::string_view name, glm::vec3 val) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      CheckUniformExistence(uniform_loc, name);

      glUniform3fv(uniform_loc, 1, glm::value_ptr(val));
    }

    inline void SetUniformVec4f(std::string_view name,
                                float val0, float val1,
                                float val2, float val3) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      CheckUniformExistence(uniform_loc, name);

      glUniform4f(uniform_loc, val0, val1, val2, val3);
    }

    inline void SetUniformVec4f(std::string_view name, glm::vec4 val) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      CheckUniformExistence(uniform_loc, name);

      glUniform4fv(uniform_loc, 1, glm::value_ptr(val));
    }

    inline void SetUniformMat4f(std::string_view name, glm::mat4 mat) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      CheckUniformExistence(uniform_loc, name);

      glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    inline void SetUniformInt(std::string_view name, int val) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      CheckUniformExistence(uniform_loc, name);

      glUniform1i(uniform_loc, val);
    }

    inline void SetUniformFloat(std::string_view name, float val) const
    {
      int uniform_loc{ glGetUniformLocation(m_prog, name.data()) };

      CheckUniformExistence(uniform_loc, name);

      glUniform1f(uniform_loc, val);
    }

  private:
    GLuint m_prog;

    void CheckError(GLuint id, bool is_program) const;
    void CheckUniformExistence(GLint loc, std::string_view name) const;
  };
}
