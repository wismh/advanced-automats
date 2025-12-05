#pragma once
#include <memory>

#include "./SimulationBuffer.h"

namespace Program {
    class SimulationView {
    public:
        virtual ~SimulationView() = default;

        virtual void SetBuffer(const std::shared_ptr<SimulationBuffer>& buffer) = 0;
        virtual void Draw(int offsetY) = 0;
    };
}