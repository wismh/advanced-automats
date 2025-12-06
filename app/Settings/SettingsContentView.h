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

    static void FieldScalar(const std::string& name, const std::string& id, const ImGuiDataType type, void* pData) {
        ImGui::Text(name.c_str());
        ImGui::NextColumn();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputScalar(id.c_str(), type, pData);
        ImGui::NextColumn();
    }

    static void FieldBool(const std::string& name, const std::string& id, bool* pData) {
        ImGui::Text(name.c_str());
        ImGui::NextColumn();
        ImGui::SetNextItemWidth(-1);
        ImGui::Checkbox(id.c_str(), pData);
        ImGui::NextColumn();
    }
};

}
