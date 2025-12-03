#include "Application.h"

using namespace Program;

const float ControlPanelView::k_buttonWidth = 100;

int main()
{
    Application app(1280, 960, "Cellular Automata Simulations");
    return app.Loop();
}
