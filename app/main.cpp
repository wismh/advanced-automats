#include "Application/Application.h"
#include "SimulationFlow/SimulationFlowPanelView.h"

using namespace Program;

const float SimulationFlowPanelView::k_buttonWidth = 100;

int main()
{
    Application app(1280, 960, "Cellular Automata Simulations");
    return app.Loop();
}
