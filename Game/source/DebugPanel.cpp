#include "LPSPCH.hpp"
#include "DebugPanel.hpp"

namespace LPS
{
  DebugPanel::DebugPanel()
    : visible(false)
    , clear_color(0.2f, 0.2f, 0.2f)
    , sleep_count(10)
  {}

  void DebugPanel::Draw()
  {
    if (visible){
      ImGuiIO& io{ ImGui::GetIO() };

      ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, io.DisplaySize.y),
                                          ImVec2(500.0f, io.DisplaySize.y));
      ImGui::SetNextWindowSize(ImVec2(300.0f, io.DisplaySize.y),
                              ImGuiCond_FirstUseEver);
      ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
      ImGui::SetNextWindowBgAlpha(1.0f);

      ImGuiWindowFlags flags{ ImGuiWindowFlags_NoCollapse };

      static bool show_demo{ false };

      ImGui::Begin("Debug Panel", nullptr, flags);

      ImGui::Text("Average FPS: %.1f", io.Framerate);
      ImGui::Text("Time running: %.1fs", glfwGetTime());
      ImGui::Dummy(ImVec2(0.0f, 20.0f));
      ImGui::Text("Clear Color");
      ImGui::ColorEdit3("##", (float*)&clear_color);
      ImGui::Dummy(ImVec2(0.0f, 20.0f));
      ImGui::Text("Thread Sleep Count (ms)");
      ImGui::SliderInt("##", &sleep_count, 0, 50);
      ImGui::Dummy(ImVec2(0.0f, 20.0f));

      ImGui::Checkbox("Show Demo Window", &show_demo);

      if (show_demo)
      {
        ImGui::ShowDemoWindow(&show_demo);
      }

      ImGui::End();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Space))
    {
      visible = !visible;
    }
  }
}
