#pragma once
#include <raylib.h>

#include "ExtendedWolframSimulationBuffer.h"
#include "../../Simulation/SimulationView.h"

namespace Program {
    class ExtendedWolframSimulationView final : public SimulationView {
        std::shared_ptr<ExtendedWolframSimulationBuffer> _buffer;
        std::shared_ptr<ExtendedWolframSimulationConfig> _config;

        Image _image;
        Texture _texture;

        int _capacityWidth;
        int _capacityHeight;

        void GenTexture() {
            UnloadTexture(_texture);
            UnloadImage(_image);

            _image = GenImageColor(_capacityWidth, _capacityHeight, BLACK);
            _texture = LoadTextureFromImage(_image);
        }
    public:
        ExtendedWolframSimulationView(
            const std::shared_ptr<ExtendedWolframSimulationConfig>& config,
            const std::shared_ptr<ExtendedWolframSimulationBuffer>& buffer
        ) {
            _config = config;
            _buffer = std::dynamic_pointer_cast<ExtendedWolframSimulationBuffer>(buffer);

            _capacityWidth = 512;
            _capacityHeight = 512;

            _image = GenImageColor(_capacityWidth, _capacityHeight, BLACK);
            _texture = LoadTextureFromImage(_image);

            buffer->OnClear = std::bind(&ExtendedWolframSimulationView::GenTexture, this);
        }

        void Draw() override {
            const auto buffer =  _buffer->GetBuffer();

            if (_buffer->MaxWidth() >= _capacityWidth) {
                _capacityWidth = _buffer->MaxWidth() * 1.5f;
                GenTexture();
            }

            if (_buffer->Height() >= _capacityHeight) {
                _capacityHeight = _buffer->Height() * 1.5f;
                GenTexture();
            }

            const auto pixels = static_cast<Color*>(_image.data);
            for (int y = 0; y < buffer.size(); ++y)
                for (int x = 0; x < buffer[y].size(); ++x)
                    pixels[y * _capacityWidth + x] = (buffer[y][x] == 1) ? BLUE : BLACK;

            UpdateTexture(_texture, _image.data);
            DrawTexture(_texture, 0, 0, WHITE);
        }
    };
}