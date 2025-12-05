#pragma once
#include <raylib.h>

namespace Program {

class IPointerDownHandler {
public:
    virtual ~IPointerDownHandler() = default;
    virtual void HandlePointerDown(const Vector2& position) = 0;
};

}
