#pragma once
#include <memory>
#include <rlImGui.h>

#include "ApplicationControls.h"
#include "ApplicationModel.h"
#include "SimulationFlow/SimulationFlowPanelView.h"
#include "Settings/SettingsPanelView.h"
#include "SimulationFlow/SimulationFlowConfig.h"
#include "Simulation/SimulationView.h"
#include "Camera/CameraControls.h"
#include "Timer.h"

#include "Simulations/ExtendedWolfram/ExtendedWolframSettingsView.h"
#include "Simulations/ExtendedWolfram/ExtendedWolframSimulation.h"
#include "Simulations/ExtendedWolfram/ExtendedWolframSimulationView.h"

namespace Program {

class Application final {
    std::shared_ptr<ApplicationModel> _applicationModel;

    std::shared_ptr<SettingsPanelView> _settingsPanelView;
    std::shared_ptr<SimulationFlowPanelView> _controlPanelView;

    std::shared_ptr<SimulationView> _simulationView;

    std::shared_ptr<CameraControls>  _cameraControls;
    std::shared_ptr<ApplicationControls> _applicationControls;
    std::shared_ptr<ExtendedWolframSimulation> _simulation;

    std::vector<std::shared_ptr<ITickable>> _tickables{};
    std::vector<std::shared_ptr<IPointerDownHandler>> _pointerDownHandlers{};
    std::vector<std::shared_ptr<IPointerUpHandler>> _pointerUpHandlers{};

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

        for (const auto& tickable : _tickables)
            tickable->Tick();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            const auto& position = GetMousePosition();
            for (const auto& handler : _pointerDownHandlers)
                handler->HandlePointerDown(position);
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            const auto& position = GetMousePosition();
            for (const auto& handler : _pointerUpHandlers)
                handler->HandlePointerUp(position);
        }
    }

    void Draw() const {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(*_applicationModel->CameraModel);
        _simulationView->Draw();
        EndMode2D();

        rlImGuiBegin();

        _settingsPanelView->Draw();
        _controlPanelView->Draw();

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
        _applicationModel->CameraModel = std::make_shared<Camera2D>();
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

        _cameraControls = std::make_shared<CameraControls>(
            _applicationModel->CameraModel
        );
        _tickables.emplace_back(_cameraControls);
        _pointerDownHandlers.emplace_back(_cameraControls);
        _pointerUpHandlers.emplace_back(_cameraControls);

        std::cout << "[Application] Init Controls Components Successfully" << std::endl;

        // Views
        _settingsPanelView = std::make_shared<SettingsPanelView>(300);
        _settingsPanelView->SetContentView(std::make_shared<ExtendedWolframSettingsView>(
            std::static_pointer_cast<ExtendedWolframSimulationConfig>(_applicationModel->SimulationConfig)
        ));
        _controlPanelView = std::make_shared<SimulationFlowPanelView>(
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
