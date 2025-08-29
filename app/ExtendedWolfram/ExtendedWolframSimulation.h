#pragma once
#include <memory>
#include <complex>

namespace Program {

    class ExtendedWolframSimulation {
        std::shared_ptr<int[]> _state;
        std::shared_ptr<int[]> _updatedState;

        std::shared_ptr<int[]> _neighbors;

        int _width;
        int _neighborsCount;
        int _casesCount;
        unsigned long long _rule;

        void UpdateState(int p, int j) const {
            const int half = _neighborsCount / 2;
            bool fits = true;

            for (int i = 0; i < _neighborsCount; i++)
                if ((*this)[p - i + half] != _neighbors[j * _neighborsCount + i]) {
                    fits = false;
                    break;
                }

            if (fits)
                _updatedState[p] = (_rule >> j) & 1;
        }
    public:
        ExtendedWolframSimulation(
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

            _casesCount = std::pow(2, _neighborsCount);
            _neighbors = std::shared_ptr<int[]>(new int[_casesCount * _neighborsCount]);

            for (int j = 0; j < _casesCount; ++j)
                for (int k = 0; k < _neighborsCount; ++k)
                    _neighbors[j * _neighborsCount + k] = (j >> k) & 1;
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
            for (int i = 0; i < _width; ++i)
                for (int j = 0; j < _casesCount; ++j)
                    UpdateState(i, j);
        }
    };
}
