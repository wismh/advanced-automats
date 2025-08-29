#include <raylib.h>

#include "SimulationView.h"
#include "Timer.h"

import ExtendedWolfram;

using namespace Program;

int main() {
    const auto config = ProgramConfig(
        1280,
        880,
        4,
        4,
        500
    );

    constexpr int neighborCount = 6;
    constexpr bool isRandomFirst = true;
    unsigned long long rule = 0;

    std::random_device rd;
    std::mt19937_64 gen(rd());

    auto buffer = std::make_shared<ExtendedWolframSimulationBuffer>(
        config,
        neighborCount,
        rule,
        isRandomFirst
    );

    std::unique_ptr<SimulationView> simulationView =
        std::make_unique<ExtendedWolframSimulationView>(config, buffer);

    Timer timer;

    InitWindow(
        config.WindowWidth(),
        config.WindowHeight(),
        "May force be with you"
    );

    while (!WindowShouldClose()) {
        if (timer.HasElapsed()) {
            rule = gen();

            buffer = std::make_shared<ExtendedWolframSimulationBuffer>(
                config,
                neighborCount,
                rule,
                isRandomFirst
            );

            while (!buffer->IsFull())
                buffer->CalcNextState();

            simulationView->SetBuffer(buffer);

            timer.SetTimeout(config.Delay());
        }

        BeginDrawing();
        ClearBackground(WHITE);

        simulationView->Draw(32);

        DrawText(
            std::to_string(rule).c_str(),
            10,
            0,
            32,
            RED
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
