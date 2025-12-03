#pragma once
#include <memory>
#include <complex>

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

class ExtendedWolframSimulation {
    std::shared_ptr<int[]> _state;
    std::shared_ptr<int[]> _updatedState;

    std::shared_ptr<int[]> _neighbors;

    int _casesCount;
    std::shared_ptr<ExtendedWolframSimulationConfig> _config;

    void UpdateState(const int p, const int j) const {
        const int half = _config->Neighbors / 2;
        bool fits = true;

        for (int i = 0; i <  _config->Neighbors; i++)
            if ((*this)[p - i + half] != _neighbors[j *  _config->Neighbors + i]) {
                fits = false;
                break;
            }

        if (fits)
            _updatedState[p] = (_config->Rule >> j) & 1;
    }
public:
    ExtendedWolframSimulation(
        const std::shared_ptr<ExtendedWolframSimulationConfig> config,
        const std::shared_ptr<int[]> &state,
        const std::shared_ptr<int[]> &updatedState
    ) :
        _config(config)
    {
        SetStatePtr(state);
        SetUpdatedStatePtr(updatedState);

        _casesCount = std::pow(2, _config->Neighbors);
        _neighbors = std::shared_ptr<int[]>(new int[_casesCount * _config->Neighbors]);

        for (int j = 0; j < _casesCount; ++j)
            for (int k = 0; k < _config->Neighbors; ++k)
                _neighbors[j * _config->Neighbors + k] = (j >> k) & 1;
    }

    void SetStatePtr(const std::shared_ptr<int[]>& state) {
        if (!state)
            throw std::invalid_argument("Passed state is null");

        _state = state;
    }

    void SetUpdatedStatePtr(const std::shared_ptr<int[]>& updatedState) {
        if (!updatedState)
            throw std::invalid_argument("Passed state is null");

        _updatedState = updatedState;
    }

    std::shared_ptr<int[]> GetState() const {
        return _state;
    }

    int operator[](const int index) const {
        const int idx = (index + _config->Width) % _config->Width;
        return _state[idx];
    }

    void CalcNextState() const {
        for (int i = 0; i < _config->Width; ++i)
            for (int j = 0; j < _casesCount; ++j)
                UpdateState(i, j);
    }
};

}
