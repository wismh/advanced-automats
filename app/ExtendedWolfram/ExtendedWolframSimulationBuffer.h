#pragma once
#include <random>
#include <vector>

#include "ExtendedWolframSimulation.h"
#include "../ProgramConfig.h"
#include "../SimulationBuffer.h"

namespace Program {



class ExtendedWolframSimulationBuffer final : public SimulationBuffer {
    std::unique_ptr<ExtendedWolframSimulation> _simulation;
    std::vector<std::vector<int>> _buffer;
    std::shared_ptr<ExtendedWolframSimulationConfig> _config;

    int _currentLine;

    static void InitRandomBuffer(const int width, std::vector<int>& state) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);

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
        _currentLine = 0;
        InitRandomBuffer(config->Width, _buffer[0]);

        _simulation = std::make_unique<ExtendedWolframSimulation>(
            config,
            std::shared_ptr<int[]>(_buffer[0].data()),
            std::shared_ptr<int[]>(_buffer[1].data())
        );
    }

    [[nodiscard]] bool IsFull() const override {
        return _currentLine >= _buffer.size() - 1;
    }

    void CalcNextState() override {
        if (IsFull())
            return;

        _currentLine += 1;

        _simulation->SetStatePtr(std::shared_ptr<int[]>(_buffer[_currentLine - 1].data()));
        _simulation->SetUpdatedStatePtr(std::shared_ptr<int[]>(_buffer[_currentLine].data()));

        _simulation->CalcNextState();
    }

    std::vector<std::vector<int>>& GetBuffer() {
        return _buffer;
    }
};

}
