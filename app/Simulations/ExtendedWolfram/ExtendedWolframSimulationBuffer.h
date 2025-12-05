#pragma once
#include <random>
#include <vector>

#include "../../Simulation/SimulationBuffer.h"

namespace Program {

class SimulationConfig {
public:
    virtual ~SimulationConfig() = default;
};

class ExtendedWolframSimulationConfig final : public SimulationConfig {
public:
    int Width = 64;
    int Neighbors = 3;
    int Rule = 90;
    bool IsLoop = true;
};

class ExtendedWolframSimulationBuffer final : public SimulationBuffer {
    friend class ExtendedWolframSimulation;

    std::vector<std::vector<int>> _buffer;
    std::shared_ptr<ExtendedWolframSimulationConfig> _config;

    int _currentLine;

    static void InitRandomBuffer(const int width, std::vector<int>& state) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        state.resize(width);
        for (int i = 0; i < width; ++i)
            state[i] = dist(gen);
    }

    static void InitCentralBuffer(const int width, std::vector<int>& state) {
        std::fill_n(state.data(), width, 0);
        state[width / 2] = 1;
    }
public:
    explicit ExtendedWolframSimulationBuffer(
        const std::shared_ptr<ExtendedWolframSimulationConfig>& config
    ):
        _config(config)
    {
        Clear();
    }

    void Clear() override {
        _currentLine = 0;

        _buffer.clear();
        _buffer.push_back({});
        InitRandomBuffer(_config->Width, _buffer[0]);
    }

    std::vector<std::vector<int>>& GetBuffer() {
        return _buffer;
    }
};

}
