#include <imgui_internal.h>
#include <iostream>
#include <memory>
#include <string>

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Timer.h"
#include "ExtendedWolfram/ExtendedWolframSimulationView.h"

using namespace Program;

class SimulationFlowConfig final {
public:
    int Speed = 1;
    bool IsPaused = true;
};

class CameraModel final {
public:
    float Zoom = 0.f;
    ImVec2 Position = {};
};

class ApplicationModel final {
public:
    std::shared_ptr<SimulationBuffer> SimulationBuffer;
    std::shared_ptr<SimulationFlowConfig> SimulationFlowConfig;
    std::shared_ptr<SimulationConfig> SimulationConfig;
    std::shared_ptr<CameraModel> CameraModel;
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

class ControlPanelView final {
    static const float k_buttonWidth;

    float _width;
    float _height;
    float _buttonSize;
    ImGuiContext* _context = nullptr;

    std::shared_ptr<SettingsPanelView> _settingsPanelView;
    std::shared_ptr<ApplicationModel> _applicationModel;
public:
    explicit ControlPanelView(
        const int height,
        const std::shared_ptr<SettingsPanelView>& settingsPanelView,
        const std::shared_ptr<ApplicationModel>& applicationModel
    ) {
        _height = static_cast<float>(height);
        _settingsPanelView = settingsPanelView;
        _applicationModel = applicationModel;

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
        DrawClearButton();
        DrawExitButton();
        DrawSpeedSlider();

        ImGui::End();
    }
private:
    void DrawPlayPause() const {
        ImGui::SameLine();
        ImGui::SetCursorPosY(_context->Style.WindowPadding.y);
        TogglePlayPause(_applicationModel->SimulationFlowConfig->IsPaused, _buttonSize);
    }

    void DrawClearButton() const {
        ImGui::SameLine();
        ImGui::SetCursorPosY(_context->Style.WindowPadding.y);
        ImGui::Button("Clear", ImVec2(k_buttonWidth, _buttonSize));
    }

    void DrawExitButton() const {
        ImGui::SameLine();
        ImGui::SetCursorPosY(_context->Style.WindowPadding.y);
        ImGui::Button("Exit", ImVec2(k_buttonWidth, _buttonSize));
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

const float ControlPanelView::k_buttonWidth = 100;

class Application final {
    std::shared_ptr<ApplicationModel> _applicationModel;

    std::shared_ptr<SettingsPanelView> _settingsPanelView;
    std::shared_ptr<ControlPanelView> _controlPanelView;

    std::shared_ptr<SimulationView> _simulationView;

    Timer _timer;
public:
    Application(
        const int windowWidth,
        const int windowHeight,
        const std::string& windowTitle
    ) {
        InitImGui(windowWidth, windowHeight, windowTitle);
        InitComponents();
    }

    ~Application() {
        rlImGuiShutdown();
        CloseWindow();
    }

    int Loop() {
        while (!WindowShouldClose())
        {
            Update();
            Draw();
        }

        return 0;
    }
private:
    void Update() {
        const auto buffer = _applicationModel->SimulationBuffer;
        const auto flowConfig =_applicationModel->SimulationFlowConfig;

        if (_timer.HasElapsed() && !flowConfig->IsPaused) {
            buffer->CalcNextState();
            _timer.SetTimeout(2000 - flowConfig->Speed * 20);
        }
    }

    void Draw() const {
        BeginDrawing();
        ClearBackground(BLACK);

        rlImGuiBegin();

        _settingsPanelView->Draw();
        _controlPanelView->Draw();
        _simulationView->Draw(50);

        rlImGuiEnd();
        EndDrawing();
    }
private:
    static void InitImGui(
        const int windowWidth,
        const int windowHeight,
        const std::string& windowTitle
    ) {
        InitWindow(windowWidth, windowHeight, windowTitle.c_str());
        SetTargetFPS(60);

        rlImGuiSetup(true);

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;

        std::cout << "[Application] Init ImGui Successfully" << std::endl;
    }

    void InitComponents() {
        // Data
        _applicationModel = std::make_shared<ApplicationModel>();
        _applicationModel->SimulationFlowConfig = std::make_shared<SimulationFlowConfig>();
        _applicationModel->SimulationConfig = std::make_shared<ExtendedWolframSimulationConfig>();
        _applicationModel->SimulationBuffer = std::make_shared<ExtendedWolframSimulationBuffer>(
            std::dynamic_pointer_cast<ExtendedWolframSimulationConfig>(_applicationModel->SimulationConfig)
        );

        std::cout << "[Application] Init Data Components Successfully" << std::endl;

        // Views
        _settingsPanelView = std::make_shared<SettingsPanelView>(300);
        _settingsPanelView->SetContentView(std::make_shared<ExtendedWolframSettingsView>());
        _controlPanelView = std::make_shared<ControlPanelView>(
            50, _settingsPanelView, _applicationModel
        );
        _simulationView = std::make_shared<ExtendedWolframSimulationView>(
            std::static_pointer_cast<ExtendedWolframSimulationConfig>(_applicationModel->SimulationConfig),
            std::static_pointer_cast<ExtendedWolframSimulationBuffer>(_applicationModel->SimulationBuffer)
        );

        std::cout << "[Application] Init Views Components Successfully" << std::endl;
    }
};

int main()
{
    Application app(1280, 960, "Cellular Automata Simulations");
    return app.Loop();
}
