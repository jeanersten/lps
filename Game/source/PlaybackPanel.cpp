#include "LPSPCH.hpp"
#include "PlaybackPanel.hpp"

namespace LPS
{
  PlaybackPanel::PlaybackPanel()
    : visible(true)
    , left_padding(0.0f)
    , simulation(nullptr)
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
        ImVec2{ io.DisplaySize.x, 360.0f },
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

      ParabolicSimulation::State state{ simulation->GetState() };

      std::string button_text{  };

      if (state == ParabolicSimulation::PS_STATE_PAUSE)
      {
        button_text = "Play";
      }
      else
      {
        button_text = "Pause";
      }

      if (ImGui::Button(button_text.c_str(), ImVec2{ 80.0f, 30.0f }))
      {
        if (state == ParabolicSimulation::PS_STATE_PAUSE)
        {
          state = ParabolicSimulation::PS_STATE_PLAY;
        }
        else
        {
          state = ParabolicSimulation::PS_STATE_PAUSE;
        }

        simulation->SetState(state);
      }

      ImGui::SameLine();

      if (ImGui::Button("Reset", ImVec2{ 80.0f, 30.0f }))
      {
        simulation->Reset();
      }

      ImGui::SameLine();

      // Slider
      float current_time{ simulation->time };
      ImGui::SetNextItemWidth(-1.0f);
      if (ImGui::SliderFloat("##simtime", &current_time,
                             0.0f, simulation->total_time, "at %.1fs"))
      {
        simulation->time = current_time;
        simulation->CalcPosition(current_time);
      }

      ImGui::Separator();
      ImGui::Dummy({ 0.0f, 10.0f});

      float input_width{ 96.0f };

      // Initial position
      glm::vec3 init_pos{ simulation->ipos };
      ImGui::Text("X Init:"); ImGui::SameLine();
      ImGui::SetNextItemWidth(input_width);
      if (ImGui::InputFloat("##bposx", &init_pos.x, 0.0f, 0.0f, "%.1f"))
      {
        simulation->ipos = init_pos;
        simulation->Reset();
      }
      ImGui::SameLine();
      ImGui::Text("Y Init:"); ImGui::SameLine();
      ImGui::SetNextItemWidth(input_width);
      if (ImGui::InputFloat("##bposy", &init_pos.y, 0.0f, 0.0f, "%.1f"))
      {
        simulation->ipos = init_pos;
        simulation->Reset();
      }
      ImGui::SameLine();
      ImGui::Text("Z Init:"); ImGui::SameLine();
      ImGui::SetNextItemWidth(input_width);
      if (ImGui::InputFloat("##bposz", &init_pos.z, 0.0f, 0.0f, "%.1f"))
      {
        simulation->ipos = init_pos;
        simulation->Reset();
      }

      ImGui::Dummy({ 0.0f, 5.0f});

      // Initial velocity
      float init_vel{ simulation->ivel };
      ImGui::Text("V Init:"); ImGui::SameLine();
      ImGui::SetNextItemWidth(input_width);
      if (ImGui::InputFloat("##bvel", &init_vel, 0.0f, 0.0f, "%.1f m/s"))
      {
        simulation->ivel = init_vel;
        simulation->Reset();
      }

      ImGui::Dummy({ 0.0f, 5.0f});

      // Launch angle
      float angle{ simulation->langle };
      ImGui::Text("Launch Angle:"); ImGui::SameLine();
      ImGui::SetNextItemWidth(input_width);
      if (ImGui::InputFloat("##bang", &angle, 0.0f, 0.0f, "%.1f deg"))
      {
        simulation->langle = angle;
        simulation->Reset();
      }

      // Gravity
      float gravity{ simulation->grv };
      ImGui::Text("Gravity     :"); ImGui::SameLine();
      ImGui::SetNextItemWidth(input_width);
      if (ImGui::InputFloat("##grv", &gravity, 0.0f, 0.0f, "%.1f m/s"))
      {
        simulation->grv = gravity;
        simulation->Reset();
      }

      ImGui::End();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Space))
    {
      visible = !visible;
    }
  }
}
