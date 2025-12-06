#pragma once
#include <memory>
#include <complex>
#include <iostream>

#include "ExtendedWolframSimulationBuffer.h"

namespace Program {

class ExtendedWolframSimulation final {
    std::shared_ptr<ExtendedWolframSimulationBuffer> _buffer;
    std::shared_ptr<ExtendedWolframSimulationConfig> _config;

    int _casesCount;

    void UpdateState(const int p, const int j) const {
        const int half = _config->Neighbors / 2;
        bool fits = true;

        for (int i = 0; i < _config->Neighbors; ++i)
            if ((*this)[p - i + half] != (j >> i & 1)) {
                fits = false;
                break;
            }

        if (fits)
            SetValue(p, _config->Rule >> j & 1);
    }

    void SetValue(const int position, const int value) const {
        auto& buffer = _buffer->_buffer[_buffer->_currentLine];
        if (position < 0 || position >= buffer.size())
            return;

        buffer[position] = value;
    }

    int operator[](const int index) const {
        const auto buffer = _buffer->_buffer[_buffer->_currentLine - 1];

        if (_config->IsLoop) {
            const int idx = (index + buffer.size()) % buffer.size();
            return buffer[idx];
        }

        if (index < 0 || index >= buffer.size())
            return 0;

        return buffer[index];
    }
public:
    ExtendedWolframSimulation(
        const std::shared_ptr<ExtendedWolframSimulationConfig>& config,
        const std::shared_ptr<ExtendedWolframSimulationBuffer>& buffer
    ) :
        _buffer(buffer),
        _config(config)
    {
        _casesCount = std::pow(2, _config->Neighbors);
    }

    void CalcNextState() const {;
        _buffer->_currentLine += 1;
        _buffer->_buffer.emplace_back(_config->Width);

        for (int i = 0; i < _config->Width; ++i)
            for (int j = 0; j < _casesCount; ++j)
                UpdateState(i, j);
    }
};

}
