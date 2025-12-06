#include "LPSPCH.hpp"
#include "Log.hpp"
#include "Shader.hpp"

namespace LPS
{
  Shader::Shader(const std::filesystem::path& vshader_path,
                 const std::filesystem::path& fshader_path)
    : m_prog(0)
  {
    std::ifstream vshader_file{ std::filesystem::absolute(vshader_path) };
    std::ifstream fshader_file{ std::filesystem::absolute(fshader_path) };

    LPS_ASSERT(vshader_file.is_open(),
               "Failed to open vertex shader file.");
    LPS_ASSERT(fshader_file.is_open(),
               "Failed to open fragment shader file.");

    std::stringstream vshader_stream{ };
    std::stringstream fshader_stream{ };

    vshader_stream << vshader_file.rdbuf();
    fshader_stream << fshader_file.rdbuf();

    vshader_file.close();
    fshader_file.close();

    std::string vshader_source{ vshader_stream.str() };
    std::string fshader_source{ fshader_stream.str() };

    const char* vshader_source_cstr{ vshader_source.c_str() };
    const char* fshader_source_cstr{ fshader_source.c_str() };

    GLuint vshader{ glCreateShader(GL_VERTEX_SHADER) };

    glShaderSource(vshader, 1, &vshader_source_cstr, NULL);
    glCompileShader(vshader);
    CheckError(vshader, false);

    GLuint fshader{ glCreateShader(GL_FRAGMENT_SHADER) };

    glShaderSource(fshader, 1, &fshader_source_cstr, NULL);
    glCompileShader(fshader);
    CheckError(fshader, false);

    m_prog = glCreateProgram();
    glAttachShader(m_prog, vshader);
    glAttachShader(m_prog, fshader);
    glLinkProgram(m_prog);
    CheckError(m_prog, true);

    glDeleteShader(vshader);
    glDeleteShader(fshader);
  }

  Shader::~Shader()
  {
    if (m_prog != 0) glDeleteProgram(m_prog);
  }

  void Shader::Use() const
  {
    glUseProgram(m_prog);
  }

  void Shader::CheckError(GLuint id, bool is_program) const
  {
    int status{ };
    char info_log[1024]{ };

    if (!is_program)
    {
      glGetShaderiv(id, GL_COMPILE_STATUS, &status);

      if (status == GL_FALSE)
      {
        glGetShaderInfoLog(id, 1024, NULL, info_log);
        LPS_LOG_ERROR("{}", info_log);
      }
    }
    else
    {
      glGetProgramiv(id, GL_LINK_STATUS, &status);

      if (status == GL_FALSE)
      {
        glGetProgramInfoLog(id, 1024, NULL, info_log);
        LPS_LOG_ERROR("{}", info_log);
      }
    }
  }
}
