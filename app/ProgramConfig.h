#pragma once

namespace Program {
    class ProgramConfig {
        int _windowWidth;
        int _windowHeight;

        int _cellWidth;
        int _cellHeight;

        int _delay;

        int _gridWidth;
        int _gridHeight;
    public:
        ProgramConfig(
            const int windowWidth,
            const int windowHeight,
            const int cellWidth,
            const int cellHeight,
            const int delay
        ):
            _windowWidth(windowWidth),
            _windowHeight(windowHeight),
            _cellWidth(cellWidth),
            _cellHeight(cellHeight),
            _delay(delay)
        {
            _gridWidth = _windowWidth / _cellWidth;
            _gridHeight = _windowHeight / _cellHeight;
        }

        int WindowWidth() const { return _windowWidth; }
        int WindowHeight() const { return _windowHeight; }

        int CellWidth() const { return _cellWidth; }
        int CellHeight() const { return _cellHeight; }

        int GridWidth() const { return _gridWidth; }
        int GridHeight() const { return _gridHeight; }

        int Delay() const { return _delay; }
    };

}