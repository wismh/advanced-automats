#include <imgui_internal.h>
#include <memory>
#include <string>

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Timer.h"
#include "ExtendedWolfram/ExtendedWolframSimulationView.h"

using namespace Program;

class SimulationFlowConfig {
public:
    int Speed = 1;
    bool IsPaused = true;
};

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

class ExtendedWolframSettingsView final : public SettingsContentView {
    int _neighbours = 0;
    int _rule = 0;
protected:
    void DrawContent() override {
        Field("Neighbours", "##neighbours", &_neighbours);
        Field("Rule", "##rule", &_rule);
    }
};

class ControlPanel final {
    static const float k_buttonWidth;

    float _height;
    std::shared_ptr<SettingsPanelView> _settingsPanelView;
    std::shared_ptr<SimulationFlowConfig> _simulationFlowConfig;
public:
    explicit ControlPanel(
        const int height,
        const std::shared_ptr<SettingsPanelView>& settingsPanelView,
        const std::shared_ptr<SimulationFlowConfig>& simulationFlowConfig
    ) {
        _height = static_cast<float>(height);
        _settingsPanelView = settingsPanelView;
        _simulationFlowConfig = simulationFlowConfig;
    }

    void Draw() const {
        const auto screenWidth = static_cast<float>(GetScreenWidth());
        const auto width = screenWidth - _settingsPanelView->Width();

        const auto context = ImGui::GetCurrentContext();
        const float buttonSize = _height - context->Style.WindowPadding.y * 2;

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(width, _height));

        ImGui::Begin("Control Panel", nullptr,
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoDecoration);

        ImGui::SameLine();
        ImGui::SetCursorPosY(context->Style.WindowPadding.y);
        TogglePlayPause(_simulationFlowConfig->IsPaused, buttonSize);

        ImGui::SameLine();
        ImGui::SetCursorPosY(context->Style.WindowPadding.y);
        ImGui::Button("Clear", ImVec2(k_buttonWidth, buttonSize));

        ImGui::SameLine();
        ImGui::SetCursorPosY(context->Style.WindowPadding.y);
        ImGui::Button("Exit", ImVec2(k_buttonWidth, buttonSize));

        ImGui::SameLine();
        static const float textWidth = ImGui::CalcTextSize("Speed:").x + 200 + 25;
        ImGui::SetCursorPosX(width - textWidth);
        ImGui::SetCursorPosY((_height - ImGui::GetFrameHeight()) * 0.5f);
        ImGui::Text("Speed:");

        ImGui::SameLine();
        ImGui::SetCursorPosY((_height - ImGui::GetFrameHeight()) * 0.5f);
        ImGui::PushItemWidth(200.0f);
        ImGui::SliderInt("##speed", &_simulationFlowConfig->Speed, 1, 100);
        ImGui::PopItemWidth();

        ImGui::End();
    }
private:
    static bool TogglePlayPause(bool& playing, const float size) {
        const char* icon = playing ? "Play" : "Pause";

        if (ImGui::Button(icon, ImVec2(k_buttonWidth, size)))
            playing = !playing;

        return playing;
    }
};

const float ControlPanel::k_buttonWidth = 100;

int main()
{
    InitWindow(1280, 960, "Cellular Automata Simulations");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    auto flowConfig = std::make_shared<SimulationFlowConfig>();

    SettingsPanelView settingsPanelView(300);
    settingsPanelView.SetContentView(std::make_shared<ExtendedWolframSettingsView>());

    const ControlPanel controlPanel(
        50,
        std::make_shared<SettingsPanelView>(settingsPanelView),
        flowConfig
    );

    const ProgramConfig config(1280, 960, 32, 32, 10);

    auto buffer = std::make_shared<ExtendedWolframSimulationBuffer>(
       config,
       3,
       90,
       true
   );

    const std::unique_ptr<SimulationView> simulationView =
        std::make_unique<ExtendedWolframSimulationView>(config, buffer);

    Timer timer;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        rlImGuiBegin();

        controlPanel.Draw();
        settingsPanelView.Draw();

        if (timer.HasElapsed() && !flowConfig->IsPaused) {
            buffer->CalcNextState();
            timer.SetTimeout(1000 - flowConfig->Speed * 10);
        }

        simulationView->Draw(50);

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
