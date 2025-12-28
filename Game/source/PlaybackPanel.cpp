#include "LPSPCH.hpp"
#include "PlaybackPanel.hpp"

namespace LPS
{
  PlaybackPanel::PlaybackPanel()
    : visible(true)
    , left_padding(0.0f)
    , m_size(0.0f)
  {}

  void PlaybackPanel::Draw()
  {
    if (visible) {
      ImGuiIO& io{ ImGui::GetIO() };

      ImGui::SetNextWindowPos(
        ImVec2{ left_padding, io.DisplaySize.y },
        ImGuiCond_Always,
        ImVec2{ 0.0f, 1.0f }
      );
      ImGui::SetNextWindowSizeConstraints(
        ImVec2{ io.DisplaySize.x - left_padding, 200.0f },
        ImVec2{ io.DisplaySize.x - left_padding, 500.0f }
      );
      ImGui::SetNextWindowSize(
        ImVec2{ io.DisplaySize.x, 300.0f },
                ImGuiCond_FirstUseEver
      );
      ImGui::SetNextWindowBgAlpha(1.0f);

      ImGuiWindowFlags flags{
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove
      };

      ImGui::Begin("Playback Panel", nullptr, flags);

      m_size.x = ImGui::GetWindowWidth();
      m_size.y = ImGui::GetWindowHeight();

      ImGui::End();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Space))
    {
      visible = !visible;
    }
  }
}
