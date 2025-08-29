#pragma once
#include <raylib.h>

#include "ExtendedWolframSimulationBuffer.h"
#include "../SimulationView.h"

namespace Program {
    class ExtendedWolframSimulationView final : public SimulationView {
        std::shared_ptr<ExtendedWolframSimulationBuffer> _buffer;
        ProgramConfig _config;
    public:
        ExtendedWolframSimulationView(
            const ProgramConfig& config,
            const std::shared_ptr<ExtendedWolframSimulationBuffer>& buffer
        ) :
            _config(config)
        {
            SetBuffer(buffer);
        }

        void SetBuffer(const std::shared_ptr<SimulationBuffer>& buffer) override {
            _buffer = std::dynamic_pointer_cast<ExtendedWolframSimulationBuffer>(buffer);
        }

        void Draw(int offsetY) override {
            const auto buffer =  _buffer->GetBuffer();
            for (int y = 0; y < buffer.size(); ++y)
                for (int x = 0; x < _config.GridWidth(); ++x)
                    if (buffer[y][x] == 1)
                        DrawRectangle(
                            x * _config.CellWidth(),
                            offsetY + y * _config.CellHeight(),
                            _config.CellWidth(),
                            _config.CellHeight(),
                            BLACK
                        );
        }
    };
}
