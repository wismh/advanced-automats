#pragma once
#include <memory>

#include "SettingsContentView.h"

namespace Program {

class SettingsPanelView final {
    float _width;
    std::shared_ptr<SettingsContentView> _contentView;
public:
    explicit SettingsPanelView(const int width) {
        _width = static_cast<float>(width);
    }

    [[nodiscard]] float Width() const {
        return _width;
    }

    void SetContentView(const std::shared_ptr<SettingsContentView>& contentView) {
        _contentView = contentView;
    }

    void Draw() const {
        const auto screenWidth = static_cast<float>(GetScreenWidth());
        const auto screenHeight = static_cast<float>(GetScreenHeight());

        ImGui::SetNextWindowPos(ImVec2(screenWidth - _width, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(_width, screenHeight));

        ImGui::Begin("Settings Panel", nullptr,
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoDecoration);

        static const float textWidth = ImGui::CalcTextSize("Simulation settings").x;
        const float avail = ImGui::GetContentRegionAvail().x;

        ImGui::SetCursorPosX((avail - textWidth) * 0.6f);
        ImGui::Text("Simulation settings");

        ImGui::Text("");

        if (_contentView != nullptr)
            _contentView->Draw();

        ImGui::End();
    }
};

}
