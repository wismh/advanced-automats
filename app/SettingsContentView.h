#pragma once
#include <imgui.h>

namespace Program {

class SettingsContentView {
    friend class SettingsPanelView;
public:
    virtual ~SettingsContentView() = default;
private:
    void Draw() {
        ImGui::Columns(2, nullptr, false);
        DrawContent();
        ImGui::Columns(1);
    }
protected:
    virtual void DrawContent() = 0;

    static void Field(const std::string& name, const std::string& id, void* pData) {
        ImGui::Text(name.c_str());
        ImGui::NextColumn();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputScalar(id.c_str(), ImGuiDataType_U32, pData);
        ImGui::NextColumn();
    }
};

}
