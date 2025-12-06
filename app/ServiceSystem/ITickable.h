#pragma once

namespace Program {

class ITickable {
public:
    virtual ~ITickable() = default;
    virtual void Tick() = 0;
};

}