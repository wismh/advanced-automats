#pragma once
#include <raylib.h>

#include "ExtendedWolframSimulationBuffer.h"
#include "../SimulationView.h"

namespace Program {
    class ExtendedWolframSimulationView final : public SimulationView {
        std::shared_ptr<ExtendedWolframSimulationBuffer> _buffer;
        std::shared_ptr<ExtendedWolframSimulationConfig> _config;
    public:
        ExtendedWolframSimulationView(
            const std::shared_ptr<ExtendedWolframSimulationConfig>& config,
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
                for (int x = 0; x < _config->Width; ++x)
                    if (buffer[y][x] == 1)
                        DrawRectangle(
                            x * 32,
                            offsetY + y * 32,
                            32,
                            32,
                            BLUE
                        );
        }
    };
}
