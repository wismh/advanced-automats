#pragma once
#include <memory>
#include <random>
#include <vector>

#include "ExtendedTotalisticSimulation.h"
#include "../ProgramConfig.h"
#include "../SimulationBuffer.h"

namespace Program {

class ExtendedTotalisticSimulationBuffer : public SimulationBuffer {
    std::unique_ptr<ExtendedTotalisticSimulation> _simulation;
    std::vector<std::shared_ptr<int[]>> _buffer;
    ProgramConfig _config;

    int _currentLine;

    static void InitRandomBuffer(const int width, const std::shared_ptr<int[]>& state) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (int i = 0; i < width; ++i)
            state[i] = dist(gen);
    }

    static void InitCentralBuffer(const int width, const std::shared_ptr<int[]>& state) {
        std::fill_n(state.get(), width, 0);
        state[width / 2] = 1;
    }
public:
    ExtendedTotalisticSimulationBuffer(
        const ProgramConfig& config,
        const int neighborsCount,
        const unsigned long long rule,
        const bool isRandomFirst
        ) : _config(config) {

        if (config.GridWidth() <= 0 || config.GridHeight() <= 0)
            throw std::invalid_argument("Invalid grid size");

        _currentLine = 0;

        _buffer.resize(config.GridHeight());
        for (auto& ptr : _buffer)
            ptr = std::shared_ptr<int[]>(new int[config.GridWidth()]{});;

        isRandomFirst ?
            InitRandomBuffer(config.GridWidth(), _buffer[0]) :
            InitCentralBuffer(config.GridWidth(), _buffer[0]);

        _simulation = std::make_unique<ExtendedTotalisticSimulation>(
            config.GridWidth(),
            neighborsCount,
            rule,
            _buffer[0],
            _buffer[1]
        );
    }

    bool IsFull() const {
        return _currentLine >= _buffer.size() - 1;
    }

    void CalcNextState() {
        if (IsFull())
            return;

        _currentLine += 1;

        _simulation->SetStatePtr(_buffer[_currentLine - 1]);
        _simulation->SetUpdatedStatePtr(_buffer[_currentLine]);

        _simulation->CalcNextState();
    }

    std::vector<std::shared_ptr<int[]>>& GetBuffer() {
        return _buffer;
    }
};

}
