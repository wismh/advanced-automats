#pragma once
#include <memory>
#include <complex>
#include <iostream>

namespace Program {

class ExtendedTotalisticSimulation {
    std::shared_ptr<int[]> _state;
    std::shared_ptr<int[]> _updatedState;

    int _width;
    int _neighborsCount;
    unsigned long long _rule;

    int CountAliveNeighbors(const int p) const {
        const int half = _neighborsCount / 2;
        int count = 0;

        for (int i = 0; i < _neighborsCount; ++i)
            count += (*this)[p - i + half];

        return count;
    }
public:
    ExtendedTotalisticSimulation(
        const int width,
        const int neighborsCount,
        const unsigned long long rule,
        const std::shared_ptr<int[]> &state,
        const std::shared_ptr<int[]> &updatedState
    ) :
        _width(width),
        _neighborsCount(neighborsCount),
        _rule(rule)
    {
        SetStatePtr(state);
        SetUpdatedStatePtr(updatedState);
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
        const int idx = (index + _width) % _width;
        return _state[idx];
    }

    void CalcNextState() const {
        for (int i = 0; i < _width; ++i) {
            const int aliveNeighbors = CountAliveNeighbors(i);
            const bool isShouldBeAlive = (_rule >> aliveNeighbors) & 1;

            _updatedState[i] = isShouldBeAlive;
        }
    }
};

}
