#pragma once
#include <memory>
#include <rlImGui.h>

#include "ApplicationControls.h"
#include "ApplicationModel.h"
#include "ControlPanelView.h"
#include "SettingsPanelView.h"
#include "SimulationFlowConfig.h"
#include "SimulationView.h"
#include "Timer.h"

#include "ExtendedWolframSettingsView.h"
#include "ExtendedWolfram/ExtendedWolframSimulation.h"
#include "ExtendedWolfram/ExtendedWolframSimulationView.h"

namespace Program {

class Application final {
    std::shared_ptr<ApplicationModel> _applicationModel;

    std::shared_ptr<SettingsPanelView> _settingsPanelView;
    std::shared_ptr<ControlPanelView> _controlPanelView;

    std::shared_ptr<SimulationView> _simulationView;

    std::shared_ptr<ApplicationControls> _applicationControls;
    std::shared_ptr<ExtendedWolframSimulation> _simulation;

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
        while (!WindowShouldClose() && _applicationModel->Running)
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
            _simulation->CalcNextState();
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

        // Controls
        _applicationControls = std::make_shared<ApplicationControls>(
            _applicationModel
        );
        _simulation = std::make_shared<ExtendedWolframSimulation>(
            std::static_pointer_cast<ExtendedWolframSimulationConfig>(_applicationModel->SimulationConfig),
            std::static_pointer_cast<ExtendedWolframSimulationBuffer>(_applicationModel->SimulationBuffer)
        );

        std::cout << "[Application] Init Controls Components Successfully" << std::endl;

        // Views
        _settingsPanelView = std::make_shared<SettingsPanelView>(300);
        _settingsPanelView->SetContentView(std::make_shared<ExtendedWolframSettingsView>(
            std::static_pointer_cast<ExtendedWolframSimulationConfig>(_applicationModel->SimulationConfig)
        ));
        _controlPanelView = std::make_shared<ControlPanelView>(
            50, _settingsPanelView, _applicationModel, _applicationControls
        );
        _simulationView = std::make_shared<ExtendedWolframSimulationView>(
            std::static_pointer_cast<ExtendedWolframSimulationConfig>(_applicationModel->SimulationConfig),
            std::static_pointer_cast<ExtendedWolframSimulationBuffer>(_applicationModel->SimulationBuffer)
        );

        std::cout << "[Application] Init Views Components Successfully" << std::endl;
    }
};

}
