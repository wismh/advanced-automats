#include <raylib.h>

#include "SimulationView.h"
#include "Timer.h"

import ExtendedTotalistic;
import ExtendedWolfram;

using namespace Program;

int main() {
    const auto config = ProgramConfig(
        1280,
        880,
        1,
        1,
        500
    );

    constexpr int neighborCount = 7;
    constexpr bool isRandomFirst = true;
    unsigned long long rule = 0;
    const unsigned long long maxRule = std::pow(2, neighborCount + 1);

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> dist(0, maxRule);

    auto buffer = std::make_shared<ExtendedTotalisticSimulationBuffer>(
        config,
        neighborCount,
        rule,
        isRandomFirst
    );

    const std::unique_ptr<SimulationView> simulationView =
        std::make_unique<ExtendedTotalisticSimulationView>(config, buffer);

    Timer timer;
    timer.SetTimeout(config.Delay());

    InitWindow(
        config.WindowWidth(),
        config.WindowHeight(),
        "May force be with you"
    );

    while (!WindowShouldClose()) {
        if (timer.HasElapsed()) {
            rule = ++rule % maxRule;

            buffer = std::make_shared<ExtendedTotalisticSimulationBuffer>(
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
