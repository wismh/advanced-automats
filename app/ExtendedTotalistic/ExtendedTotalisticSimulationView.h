#pragma once
#include <raylib.h>

#include "ExtendedTotalisticSimulationBuffer.h"

#include "../ProgramConfig.h"
#include "../SimulationView.h"

namespace Program {
    class ExtendedTotalisticSimulationView final : public SimulationView {
        std::shared_ptr<ExtendedTotalisticSimulationBuffer> _buffer;
        ProgramConfig _config;
    public:
        ExtendedTotalisticSimulationView(
            const ProgramConfig& config,
            const std::shared_ptr<ExtendedTotalisticSimulationBuffer>& buffer
        ) :
            _config(config)
        {
            SetBuffer(buffer);
        }

        void SetBuffer(const std::shared_ptr<SimulationBuffer>& buffer) override {
            _buffer = std::dynamic_pointer_cast<ExtendedTotalisticSimulationBuffer>(buffer);
        }

        void Draw(const int offsetY) override {
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