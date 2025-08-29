#pragma once
#include <chrono>

namespace Program {

class Timer {
    std::chrono::steady_clock::time_point _elapsedTime;

public:
    bool HasElapsed() const {
        return std::chrono::steady_clock::now() >= _elapsedTime;
    }

    void SetTimeout(const int milliseconds) {
        _elapsedTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
    }
};

}