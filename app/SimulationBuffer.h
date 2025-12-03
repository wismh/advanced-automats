#pragma once

namespace Program {

class SimulationBuffer {
public:
    virtual ~SimulationBuffer() = default;

    virtual void Clear() = 0;
};

}