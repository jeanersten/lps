#include "LPSPCH.hpp"
#include "InfoPanel.hpp"

namespace LPS
{
  InfoPanel::InfoPanel()
    : visible(true)
    , running_time(0.0f)
    , fps(0.0f)
    , view_pos(0.0f, 0.0f, 0.0f)
    , m_size(0.0f, 0.0f)
  {}

  void InfoPanel::Draw()
  {
    if (visible) {
      ImGuiIO& io{ ImGui::GetIO() };

      ImGui::SetNextWindowPos(
        ImVec2{ io.DisplaySize.x, 0.0f },
        ImGuiCond_Always,
        ImVec2{ 1.0f, 0.0f }
      );
      ImGui::SetNextWindowSize(
        ImVec2{ 300.0f, 200.0f },
        ImGuiCond_FirstUseEver
      );
      ImGui::SetNextWindowBgAlpha(1.0f);

      ImGuiWindowFlags flags{
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground
      };

      ImGui::Begin("Info Panel", nullptr, flags);

      m_size.x = ImGui::GetWindowWidth();
      m_size.y = ImGui::GetWindowHeight();

      fps = io.Framerate;

      auto text_right = [](const char* fmt, auto value)
        {
          char buf[64]{ };

          std::snprintf(buf, sizeof(buf), fmt, value);

          float avail{ ImGui::GetContentRegionAvail().x };
          float text_w{ ImGui::CalcTextSize(buf).x };

          ImGui::SetCursorPosX(ImGui::GetCursorPosX() + avail - text_w);
          ImGui::TextUnformatted(buf);
        };

      text_right("Average FPS: %.1f", fps);
      text_right("Time running: %.1fs", running_time);
      text_right("View X: %.1f", view_pos.x);
      text_right("View Y: %.1f", view_pos.y);
      text_right("View Z: %.1f", view_pos.z);


      ImGui::End();
    }
  }
}
