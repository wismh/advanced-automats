#pragma once
#include <memory>

#include "raylib.h"
#include "../ServiceSystem/IPointerDownHandler.h"
#include "../ServiceSystem/IPointerUpHandler.h"
#include "../ServiceSystem/ITickable.h"

namespace Program
{

class CameraControls final :
    public ITickable,
    public IPointerDownHandler,
    public IPointerUpHandler
{
private:
    std::shared_ptr<Camera2D> _cameraModel;

    Vector2 _startCameraPosition{};
    Vector2 _clickPosition{};
    Vector2 _offset{};

    bool _dragging = false;
    float _logZoom = 0;
public:
    explicit CameraControls(
        const std::shared_ptr<Camera2D>& cameraModel
    ) {
        _cameraModel = cameraModel;
        _logZoom = log(15);
    }

    void HandlePointerDown(const Vector2& position) override {
        const auto& io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return;

        _dragging = true;
        _clickPosition = position;
        _startCameraPosition = {
            _cameraModel->offset.x,
            _cameraModel->offset.y,
        };
    }

    void HandlePointerUp(const Vector2& position) override {
        _dragging = false;
    }

    void Tick() override {
        UpdateZoom();
        UpdatePosition();
    }
private:
    void UpdateZoom() {
        const Vector2 worldBefore = GetScreenToWorld2D(GetMousePosition(), *_cameraModel);

        const float wheel = GetMouseWheelMove();
        _logZoom = std::clamp(
            _logZoom + wheel/10,
            0.1f,
            3.0f
        );
        _cameraModel->zoom = exp(_logZoom);

        const Vector2 worldAfter  = GetScreenToWorld2D(GetMousePosition(), *_cameraModel);
        _cameraModel->target.x += worldBefore.x - worldAfter.x;
        _cameraModel->target.y += worldBefore.y - worldAfter.y;

    }

    void UpdatePosition() {
        if (!_dragging)
            return;

        _offset = {
            GetMousePosition().x - _clickPosition.x,
            GetMousePosition().y - _clickPosition.y
        };

        _cameraModel->offset = {
            _startCameraPosition.x + _offset.x,
            _startCameraPosition.y + _offset.y
        };
    }
};

}
