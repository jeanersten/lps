#include "LPSPCH.hpp"
#include "Log.hpp"
#include "Object3D.hpp"

namespace LPS
{
  Object3D::Object3D(glm::vec3 position, glm::vec3 size, glm::vec4 color,
                     const std::filesystem::path& texture_path)
    : m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
    , m_tex(0)
    , m_vertices()
    , m_indices()
    , m_position(position)
    , m_size(size)
    , m_color(color)
    , m_frame_mode(false)
  {
    m_vertices.reserve(24);

    float x1{ -size.x };
    float y1{  size.y };
    float z1{  size.z };
    float x2{  size.x };
    float y2{ -size.y };
    float z2{ -size.z };

    glm::vec3 front_norm{  0.0f,  0.0f,  1.0f };
    glm::vec3 back_norm {  0.0f,  0.0f, -1.0f };
    glm::vec3 left_norm { -1.0f,  0.0f,  0.0f };
    glm::vec3 right_norm{  1.0f,  0.0f,  0.0f };
    glm::vec3 top_norm  {  0.0f,  1.0f,  0.0f };
    glm::vec3 bot_norm  {  0.0f, -1.0f,  0.0f };

    // front
    m_vertices.push_back({ { x1, y1, z1 }, front_norm, color, { 0.0f, 1.0f } });
    m_vertices.push_back({ { x2, y1, z1 }, front_norm, color, { 1.0f, 1.0f } });
    m_vertices.push_back({ { x1, y2, z1 }, front_norm, color, { 0.0f, 0.0f } });
    m_vertices.push_back({ { x2, y2, z1 }, front_norm, color, { 1.0f, 0.0f } });

    // back
    m_vertices.push_back({ { x2, y1, z2 }, back_norm, color, { 0.0f, 1.0f } });
    m_vertices.push_back({ { x1, y1, z2 }, back_norm, color, { 1.0f, 1.0f } });
    m_vertices.push_back({ { x2, y2, z2 }, back_norm, color, { 0.0f, 0.0f } });
    m_vertices.push_back({ { x1, y2, z2 }, back_norm, color, { 1.0f, 0.0f } });

    // left
    m_vertices.push_back({ { x1, y1, z2 }, left_norm, color, { 0.0f, 1.0f } });
    m_vertices.push_back({ { x1, y1, z1 }, left_norm, color, { 1.0f, 1.0f } });
    m_vertices.push_back({ { x1, y2, z2 }, left_norm, color, { 0.0f, 0.0f } });
    m_vertices.push_back({ { x1, y2, z1 }, left_norm, color, { 1.0f, 0.0f } });

    // right
    m_vertices.push_back({ { x2, y1, z1 }, right_norm, color, { 0.0f, 1.0f } });
    m_vertices.push_back({ { x2, y1, z2 }, right_norm, color, { 1.0f, 1.0f } });
    m_vertices.push_back({ { x2, y2, z1 }, right_norm, color, { 0.0f, 0.0f } });
    m_vertices.push_back({ { x2, y2, z2 }, right_norm, color, { 1.0f, 0.0f } });

    // top
    m_vertices.push_back({ { x1, y1, z1 }, top_norm, color, { 0.0f, 0.0f } });
    m_vertices.push_back({ { x2, y1, z1 }, top_norm, color, { 1.0f, 0.0f } });
    m_vertices.push_back({ { x1, y1, z2 }, top_norm, color, { 0.0f, 1.0f } });
    m_vertices.push_back({ { x2, y1, z2 }, top_norm, color, { 1.0f, 1.0f } });

    // bottom
    m_vertices.push_back({ { x1, y2, z2 }, bot_norm, color, { 0.0f, 1.0f } });
    m_vertices.push_back({ { x2, y2, z2 }, bot_norm, color, { 1.0f, 1.0f } });
    m_vertices.push_back({ { x1, y2, z1 }, bot_norm, color, { 0.0f, 0.0f } });
    m_vertices.push_back({ { x2, y2, z1 }, bot_norm, color, { 1.0f, 0.0f } });


    GLuint indices[]
    {
      0,  1,  2,   2,  1,  3,
      4,  5,  6,   6,  5,  7,
      8,  9,  10,  10, 9,  11,
      12, 13, 14,  14, 13, 15,
      16, 17, 18,  18, 17, 19,
      20, 21, 22,  22, 21, 23
    };

    size_t indices_size{ sizeof(indices) / sizeof(GLuint) };

    m_vertices.reserve(indices_size);

    for (int i = 0; i < indices_size; i++)
    {
      m_indices.push_back(indices[i]);
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenTextures(1, &m_tex);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
                 m_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                 m_indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(3);

    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);

    int tex_width{ 0 };
    int tex_height{ 0 };

    stbi_set_flip_vertically_on_load(1);

    GLubyte* data{
      stbi_load(std::filesystem::absolute(texture_path).string().c_str(),
                &tex_width, &tex_height, nullptr, 3)
    };

    if (data != nullptr)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(data);
    }
    else
    {
      GLubyte red = static_cast<GLubyte>(
        std::clamp(m_color.r, 0.0f, 1.0f) * 255.0f
      );

      GLubyte green = static_cast<GLubyte>(
        std::clamp(m_color.g, 0.0f, 1.0f) * 255.0f
      );

      GLubyte blue = static_cast<GLubyte>(
        std::clamp(m_color.b, 0.0f, 1.0f) * 255.0f
      );

      GLubyte fallback_color[3]{ red, green, blue };

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, fallback_color);
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  Object3D::Object3D(glm::vec3 position, glm::vec3 size, glm::vec4 color,
                     const std::filesystem::path& model_path,
                     const std::filesystem::path& texture_path)
    : m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
    , m_tex(0)
    , m_vertices()
    , m_indices()
    , m_position(position)
    , m_size(size)
    , m_color(color)
    , m_frame_mode(false)
  {
    std::vector<glm::vec3> positions{ };
    std::vector<glm::vec3> normals{ };
    std::vector<glm::vec2> tex_coords{ };

    std::ifstream model_file(std::filesystem::absolute(model_path));

    LPS_ASSERT(model_file.is_open(),
               "Failed to open .obj model file.");

    std::string line{ };

    while (std::getline(model_file, line))
    {
      if (line.substr(0, 2) == "v ")
      {
        std::istringstream stream{ line.substr(2) };
        glm::vec3 pos{ };

        stream >> pos.x;
        stream >> pos.y;
        stream >> pos.z;

        positions.push_back(pos);
      }
      else if (line.substr(0, 2) == "vn")
      {
        std::istringstream stream{ line.substr(3) };
        glm::vec3 normal{ };

        stream >> normal.x;
        stream >> normal.y;
        stream >> normal.z;

        normals.push_back(normal);
      }
      else if (line.substr(0, 2) == "vt")
      {
        std::istringstream stream{ line.substr(3) };
        glm::vec2 tex_coord{ };

        stream >> tex_coord.x;
        stream >> tex_coord.y;

        tex_coords.push_back(tex_coord);
      }
      else if (line.substr(0, 2) == "f ")
      {
        std::istringstream stream{ line.substr(2) };

        for (size_t i = 0; i < 3; i++)
        {
          std::string str{ };

          stream >> str;

          GLuint vi = 0;
          GLuint ti = 0;
          GLuint ni = 0;

          size_t first{ str.find('/') };
          size_t second{ str.find('/', first + 1) };

          if (first == std::string::npos)
          {
            vi = std::stoi(str);
          }
          else if (second == std::string::npos)
          {
            vi = std::stoi(str.substr(0, first));
            ti = std::stoi(str.substr(first + 1));
          }
          else
          {
            vi = std::stoi(str.substr(0, first));

            if (second > first + 1)
            {
              ti = std::stoi(str.substr(first + 1, second - first - 1));
            }

            if (second + 1 < str.size())
            {
              ni = std::stoi(str.substr(second + 1));
            }
          }

          vi = vi > 0 ? vi - 1 : 0;
          ti = ti > 0 ? ti - 1 : 0;
          ni = ni > 0 ? ni - 1 : 0;

          Vertex vert{ };
          vert.pos = positions[vi];
          vert.tex_coord = tex_coords.empty()
                           ? glm::vec2{ 0.0f, 0.0f } : tex_coords[ti];
          vert.normal = normals.empty()
                        ? glm::vec3{ 0.0f, 0.0f, 1.0f } : normals[ni];
          vert.color = color;

          m_vertices.push_back(vert);
          m_indices.push_back(m_vertices.size() - 1);
        }
      }
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenTextures(1, &m_tex);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
                 m_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                 m_indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(3);

    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);

    int tex_width{ 0 };
    int tex_height{ 0 };

    stbi_set_flip_vertically_on_load(1);

    GLubyte* data{
      stbi_load(std::filesystem::absolute(texture_path).string().c_str(),
                &tex_width, &tex_height, nullptr, 3)
    };

    if (data != nullptr)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(data);
    }
    else
    {
      GLubyte red = static_cast<GLubyte>(
        std::clamp(m_color.r, 0.0f, 1.0f) * 255.0f
      );

      GLubyte green = static_cast<GLubyte>(
        std::clamp(m_color.g, 0.0f, 1.0f) * 255.0f
      );

      GLubyte blue = static_cast<GLubyte>(
        std::clamp(m_color.b, 0.0f, 1.0f) * 255.0f
      );

      GLubyte fallback_color[3]{ red, green, blue };

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, fallback_color);
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  Object3D::~Object3D()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteTextures(1, &m_tex);
  }

  void Object3D::Draw()
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glBindVertexArray(m_vao);
    glPolygonMode(GL_FRONT_AND_BACK, m_frame_mode ? GL_LINE : GL_FILL);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                   GL_UNSIGNED_INT, 0);
  }
}
