#pragma once
#include <memory>

#include "./SimulationBuffer.h"

namespace Program {
    class SimulationView {
    public:
        virtual ~SimulationView() = default;
        virtual void Draw() = 0;
    };
}