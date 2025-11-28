#include "LPSPCH.hpp"

static void glfw_error_callback(int error, const char* description)
{
  std::cout << std::format("GLFW Error {}: {}\n", error, description);
}

int main(int, char**)
{
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  const char* glsl_version = "#version 130";
  float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(
                      glfwGetPrimaryMonitor()
                     );

  GLFWwindow* window = glfwCreateWindow((int)(1280 * main_scale),
                                        (int)(800 * main_scale),
                                        "Little Physics Engine",
                                        nullptr, nullptr);

  if (window == nullptr) return -1;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  GLFWimage image{ };

  image.pixels = stbi_load("C:/Dev/lps/Game/assets/icon/lps.png",
                           &image.width, &image.height, 0, 4);

  glfwSetWindowIcon(window, 1, &image);
  stbi_image_free(image.pixels);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();

  style.ScaleAllSizes(main_scale);
  style.FontScaleDpi = main_scale;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  bool show_demo_window = true;
  bool show_another_window = false;

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  std::vector<glm::vec3> vertices = {
    { -0.5f, -0.5f, 0.0f },
    {  0.5f, -0.5f, 0.0f },
    {  0.0f,  0.5f, 0.0f }
  };

  GLuint vao{ };
  GLuint vbo{ };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  GLuint vshader{ glCreateShader(GL_VERTEX_SHADER) };
  GLuint fshader{ glCreateShader(GL_FRAGMENT_SHADER) };
  GLuint program{ glCreateProgram() };

  std::string vshader_code{ };
  std::string fshader_code{ };
  std::ifstream vshader_file{ };
  std::ifstream fshader_file{ };

  vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    vshader_file.open("C:/Dev/lps/Game/assets/shader/def_vertex.glsl");
    fshader_file.open("C:/Dev/lps/Game/assets/shader/def_fragment.glsl");

    std::stringstream vshader_stream{ };
    std::stringstream fshader_stream{ };

    vshader_stream << vshader_file.rdbuf();
    fshader_stream << fshader_file.rdbuf();
    vshader_file.close();
    fshader_file.close();
    vshader_code = vshader_stream.str();
    fshader_code = fshader_stream.str();
  }
  catch (std::ifstream::failure& e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
  }

  const char* vshader_source{ vshader_code.c_str() };
  const char* fshader_source{ fshader_code.c_str() };

  glShaderSource(vshader, 1, &vshader_source, NULL);
  glCompileShader(vshader);
  glShaderSource(fshader, 1, &fshader_source, NULL);
  glCompileShader(fshader);
  glAttachShader(program, vshader);
  glAttachShader(program, fshader);
  glLinkProgram(program);
  glDetachShader(program, vshader);
  glDetachShader(program, fshader);
  glDeleteShader(vshader);
  glDeleteShader(fshader);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
    {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!");

      ImGui::Text("This is some useful text.");
      ImGui::Checkbox("Demo Window", &show_demo_window);
      ImGui::Checkbox("Another Window", &show_another_window);

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float*)&clear_color);

      if (ImGui::Button("Button")) counter++;

      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    if (show_another_window)
    {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
        show_another_window = false;
      ImGui::End();
    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
