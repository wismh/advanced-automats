#pragma once
#include <functional>
#include <random>
#include <vector>

#include "../../Simulation/SimulationBuffer.h"
#include "./ExtendedWolframSimulationConfig.h"

namespace Program {

class ExtendedWolframSimulationBuffer final : public SimulationBuffer {
    friend class ExtendedWolframSimulation;

    std::vector<std::vector<int>> _buffer;
    std::shared_ptr<ExtendedWolframSimulationConfig> _config;

    int _currentLine;

    static void InitRandomBuffer(std::vector<int>& state) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (int i = 0; i < state.size(); ++i)
            state[i] = dist(gen);
    }

    static void InitCentralBuffer(const int width, std::vector<int>& state) {
        std::fill_n(state.data(), width, 0);
        state[width / 2] = 1;
    }
public:
    std::function<void()> OnClear;

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
        _buffer.emplace_back(_config->Width);
        InitRandomBuffer(_buffer[0]);

         if (OnClear)
            OnClear();
    }

    int MaxWidth() const {
        int maxWidth = 0;
        for (const auto& line : _buffer)
            if (line.size() > maxWidth)
                maxWidth = line.size();
        return maxWidth;
    }

    int Height() const {
        return _buffer.size();
    }

    std::vector<std::vector<int>>& GetBuffer() {
        return _buffer;
    }
};

}
