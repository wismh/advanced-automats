#pragma once

namespace Program {

class SimulationBuffer {
public:
    virtual ~SimulationBuffer() = default;

    virtual bool IsFull() const = 0;
    virtual void CalcNextState() = 0;
};

}