#pragma once
#include <imgui_internal.h>
#include "Settings/SettingsPanelView.h"

namespace Program {

class SimulationFlowPanelView final {
    static const float k_buttonWidth;

    float _width;
    float _height;
    float _buttonSize;
    ImGuiContext* _context = nullptr;

    std::shared_ptr<SettingsPanelView> _settingsPanelView;
    std::shared_ptr<ApplicationModel> _applicationModel;
    std::shared_ptr<ApplicationControls> _applicationControls;
public:
    explicit SimulationFlowPanelView(
        const int height,
        const std::shared_ptr<SettingsPanelView>& settingsPanelView,
        const std::shared_ptr<ApplicationModel>& applicationModel,
        const std::shared_ptr<ApplicationControls>& applicationControls
    ) {
        _height = static_cast<float>(height);
        _settingsPanelView = settingsPanelView;
        _applicationModel = applicationModel;
        _applicationControls = applicationControls;

        _context = ImGui::GetCurrentContext();

        const auto screenWidth = static_cast<float>(GetScreenWidth());
        _width = screenWidth - _settingsPanelView->Width();
        _buttonSize = _height - _context->Style.WindowPadding.y * 2;
    }

    void Draw() const {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(_width, _height));

        ImGui::Begin("Control Panel", nullptr,
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoDecoration);

        DrawPlayPause();
        if (DrawClearButton())
            _applicationControls->Clear();
        if (DrawExitButton())
            _applicationControls->Exit();
        if (DrawResetCameraButton())
            _applicationControls->ResetCamera();
        DrawSpeedSlider();

        ImGui::End();
    }
private:
    void DrawPlayPause() const {
        ImGui::SameLine();
        ImGui::SetCursorPosY(_context->Style.WindowPadding.y);
        TogglePlayPause(_applicationModel->SimulationFlowConfig->IsPaused, _buttonSize);
    }

    [[nodiscard]]
    bool DrawClearButton() const {
        ImGui::SameLine();
        ImGui::SetCursorPosY(_context->Style.WindowPadding.y);
        return ImGui::Button("Clear", ImVec2(k_buttonWidth, _buttonSize));
    }

    [[nodiscard]]
    bool DrawExitButton() const {
        ImGui::SameLine();
        ImGui::SetCursorPosY(_context->Style.WindowPadding.y);
        return ImGui::Button("Exit", ImVec2(k_buttonWidth, _buttonSize));
    }

    [[nodiscard]]
    bool DrawResetCameraButton() const {
        ImGui::SameLine();
        ImGui::SetCursorPosY(_context->Style.WindowPadding.y);
        return ImGui::Button("Reset Camera", ImVec2(k_buttonWidth*1.5f, _buttonSize));
    }

    void DrawSpeedSlider() const {
        static constexpr auto sliderWidth = 200;
        static const auto text = "Speed:";
        static const float textWidth = ImGui::CalcTextSize(text).x + sliderWidth + 25;
        static const float yPos = (_height - ImGui::GetFrameHeight()) * 0.5f;

        ImGui::SameLine();
        ImGui::SetCursorPosX(_width - textWidth);
        ImGui::SetCursorPosY(yPos);
        ImGui::Text(text);

        ImGui::SameLine();
        ImGui::SetCursorPosY(yPos);
        ImGui::PushItemWidth(sliderWidth);
        ImGui::SliderInt(
            "##speed",
            &_applicationModel->SimulationFlowConfig->Speed,
            1, 100
        );
        ImGui::PopItemWidth();
    }

    static bool TogglePlayPause(bool& playing, const float size) {
        const char* icon = playing ? "Play" : "Pause";

        if (ImGui::Button(icon, ImVec2(k_buttonWidth, size)))
            playing = !playing;

        return playing;
    }
};

}
