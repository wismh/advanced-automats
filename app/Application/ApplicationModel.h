#pragma once
#include "Simulation/SimulationBuffer.h"
#include "SimulationFlow/SimulationFlowConfig.h"
#include "Simulations/ExtendedWolfram/ExtendedWolframSimulationBuffer.h"

namespace Program {

class ApplicationModel final {
public:
    bool Running = true;
    std::shared_ptr<SimulationBuffer> SimulationBuffer;
    std::shared_ptr<SimulationFlowConfig> SimulationFlowConfig;
    std::shared_ptr<SimulationConfig> SimulationConfig;
    std::shared_ptr<Camera2D> CameraModel;
};

}
