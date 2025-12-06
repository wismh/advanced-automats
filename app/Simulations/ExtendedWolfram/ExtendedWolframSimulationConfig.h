#pragma once

#include "../../Simulation/SimulationConfig.h"

namespace Program {

class ExtendedWolframSimulationConfig final : public SimulationConfig {
public:
    int Width = 64;
    int Neighbors = 3;
    int Rule = 90;
    bool IsLoop = true;
};

}