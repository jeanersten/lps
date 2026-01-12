#include "LPSPCH.hpp"
#include "Camera.hpp"
#include "DebugPanel.hpp"

namespace LPS
{
  DebugPanel::DebugPanel()
    : visible(true)
    , vsync(true)
    , frame_mode(false)
    , lighting(true)
    , clear_color(0.3f, 0.3f, 0.3f)
    , cam(nullptr)
    , cam_fov(45.0f)
    , cam_speed(10.0f)
    , light_linear(0.055f)
    , light_quadratic(0.033f)
    , rot_light_dist(10.0f)
    , visibility_callback()
    , m_size(0.0f, 0.0f)
  {
    glfwSwapInterval(vsync ? 1 : 0);
  }

  void DebugPanel::Draw()
  {
    if (visible){
      ImGuiIO& io{ ImGui::GetIO() };

      ImGui::SetNextWindowPos(
        ImVec2{ 0.0f, 0.0f },
        ImGuiCond_Always,
        ImVec2{ 0.0f, 0.0f }
      );
      ImGui::SetNextWindowSizeConstraints(
        ImVec2{ 200.0f, io.DisplaySize.y },
        ImVec2{ 500.0f, io.DisplaySize.y }
      );
      ImGui::SetNextWindowSize(
        ImVec2{ 300.0f, io.DisplaySize.y },
                ImGuiCond_FirstUseEver
      );
      ImGui::SetNextWindowBgAlpha(1.0f);

      ImGuiWindowFlags flags{
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove
      };

      ImGui::Begin("Debug Panel", nullptr, flags);

      m_size.x = ImGui::GetWindowWidth();
      m_size.y = ImGui::GetWindowHeight();

      ImGui::SeparatorText("Global");
      
      if (ImGui::Checkbox("Vsync", &vsync))
      {
        glfwSwapInterval(vsync ? 1 : 0);
      }
      ImGui::Checkbox("Frame Mode", &frame_mode);
      ImGui::Text("Background Color");
      ImGui::ColorEdit3("##bgcol", (float*)&clear_color);

      ImGui::Dummy(ImVec2{ 0.0f, 20.0f });

      ImGui::SeparatorText("Camera");

      glm::vec3 cam_pos{ cam->GetPosition() };
      float cam_yaw{ cam->GetYaw() };
      float cam_pitch{ cam->GetPitch() };

      ImGui::Text("X:"); ImGui::SameLine();
      if (ImGui::InputFloat("##camx", &cam_pos.x, 0.0f, 0.0f, "%.1f"))
      {
        cam->SetPosition(cam_pos);
      }
      ImGui::Text("Y:"); ImGui::SameLine();
      if (ImGui::InputFloat("##camy", &cam_pos.y, 0.0f, 0.0f, "%.1f"))
      {
        cam->SetPosition(cam_pos);
      }
      ImGui::Text("Z:"); ImGui::SameLine();
      if (ImGui::InputFloat("##camz", &cam_pos.z, 0.0f, 0.0f, "%.1f"))
      {
        cam->SetPosition(cam_pos);
      }

      ImGui::Text("Yaw  :"); ImGui::SameLine();
      if (ImGui::InputFloat("##camyaw", &cam_yaw, 0.0f, 0.0f, "%.1f deg"))
      {
        cam->SetYaw(cam_yaw);
      }
      ImGui::Text("Pitch:"); ImGui::SameLine();
      if (ImGui::InputFloat("##campitch", &cam_pitch, 0.0f, 0.0f, "%.1f deg"))
      {
        cam->SetPitch(cam_pitch);
      }

      ImGui::Text("Field of View");
      ImGui::SliderFloat("##camfov", &cam_fov, 1.0f, 360.0f, "%1.f deg");
      ImGui::Text("Speed");
      ImGui::SliderFloat("##camspd", &cam_speed, 1.0f, 50.0f, "%1.f u/s");

      ImGui::Dummy(ImVec2{ 0.0f, 20.0f });

      ImGui::SeparatorText("Lights");
      ImGui::Checkbox("Lighting", &lighting);
      ImGui::Text("Linear");
      ImGui::SliderFloat("##linr", &light_linear, 0.0f, 0.1f);
      ImGui::Text("Quadratic");
      ImGui::SliderFloat("##quad", &light_quadratic, 0.0f, 0.1f);
      ImGui::Text("Distance");
      ImGui::SliderFloat("##rldist", &rot_light_dist, 0.0f, 50.0f, "%1.f m");

      ImGui::Dummy(ImVec2{ 0.0f, 50.0f });

      static bool show_demo_window{ false };

      ImGui::Checkbox("Show Demo Window", &show_demo_window);
      if (show_demo_window)
      {
        ImGui::ShowDemoWindow(&show_demo_window);
      }

      ImGui::End();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Space))
    {
      visible = !visible;

      if (visibility_callback) visibility_callback(visible);
    }
  }
}
