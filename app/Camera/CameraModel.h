#pragma once
#include <../../external/imgui/imgui.h>

namespace Program {

class CameraModel final {
public:
    float Zoom = 0.f;
    ImVec2 Position = {};
};

}
