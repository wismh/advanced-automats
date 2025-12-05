#pragma once
#include <raylib.h>

namespace Program {

class IPointerUpHandler {
public:
    virtual ~IPointerUpHandler() = default;
    virtual void HandlePointerUp(const Vector2& position) = 0;
};

}
