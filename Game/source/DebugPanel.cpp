#include "LPSPCH.hpp"
#include "DebugPanel.hpp"

namespace LPS
{
  DebugPanel::DebugPanel()
    : visible(true)
    , vsync(true)
    , frame_mode(false)
    , clear_color(0.2f, 0.2f, 0.2f)
    , fov(45.0f)
    , cam_speed(10.0f)
    , visibility_callback()
  {
    glfwSwapInterval(vsync ? 1 : 0);
  }

  void DebugPanel::Draw()
  {
    if (visible){
      ImGuiIO& io{ ImGui::GetIO() };

      ImGui::SetNextWindowSizeConstraints(ImVec2{ 200.0f, io.DisplaySize.y },
                                          ImVec2{ 500.0f, io.DisplaySize.y });
      ImGui::SetNextWindowSize(ImVec2{ 300.0f, io.DisplaySize.y },
                               ImGuiCond_FirstUseEver);
      ImGui::SetNextWindowPos(ImVec2{ 0.0f, 0.0f }, ImGuiCond_Always);
      ImGui::SetNextWindowBgAlpha(1.0f);

      ImGuiWindowFlags flags{ ImGuiWindowFlags_NoCollapse };

      ImGui::Begin("Debug Panel", nullptr, flags);

      ImGui::Text("Average FPS: %.1f", io.Framerate);
      ImGui::Text("Time running: %.1fs", glfwGetTime());
      ImGui::Dummy(ImVec2{ 0.0f, 20.0f });
      ImGui::Text("Clear Color");
      ImGui::ColorEdit3("##a", (float*)&clear_color);
      ImGui::Dummy(ImVec2{ 0.0f, 20.0f });
      if (ImGui::Checkbox("Vsync", &vsync))
      {
        glfwSwapInterval(vsync ? 1 : 0);
      }
      ImGui::Checkbox("Frame Mode", &frame_mode);
      ImGui::Dummy(ImVec2{ 0.0f, 20.0f });
      ImGui::Text("Field of View");
      ImGui::SliderFloat("##d", &fov, 1.0f, 360.0f);
      ImGui::Text("Camera Speed");
      ImGui::SliderFloat("##e", &cam_speed, 1.0f, 50.0f);
      ImGui::Dummy(ImVec2{ 0.0f, 20.0f });

      ImGui::End();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Space))
    {
      visible = !visible;

      if (visibility_callback) visibility_callback(visible);
    }
  }
}
