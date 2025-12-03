#pragma once
#include "CameraModel.h"
#include "SimulationBuffer.h"
#include "SimulationFlowConfig.h"
#include "ExtendedWolfram/ExtendedWolframSimulationBuffer.h"

namespace Program {

class ApplicationModel final {
public:
    bool Running = true;
    std::shared_ptr<SimulationBuffer> SimulationBuffer;
    std::shared_ptr<SimulationFlowConfig> SimulationFlowConfig;
    std::shared_ptr<SimulationConfig> SimulationConfig;
    std::shared_ptr<CameraModel> CameraModel;
};


}
