#pragma once
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
        static const auto title = "Simulation Settings";
        static const float textWidth = ImGui::CalcTextSize(title).x;
        const auto screenWidth = static_cast<float>(GetScreenWidth());
        const auto screenHeight = static_cast<float>(GetScreenHeight());

        ImGui::SetNextWindowPos(ImVec2(screenWidth - _width, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(_width, screenHeight));

        ImGui::Begin("#settings", nullptr,
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoDecoration);

        const float avail = ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX((avail - textWidth) * 0.6f);
        ImGui::Text(title);

        ImGui::Text("");

        if (_contentView != nullptr)
            _contentView->Draw();

        ImGui::End();
    }
};

}
