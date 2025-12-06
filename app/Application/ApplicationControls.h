#pragma once
#include <memory>

#include "ApplicationModel.h"

namespace Program {

class ApplicationControls final {
    std::shared_ptr<ApplicationModel> _applicationModel;
public:
    explicit ApplicationControls(
        const std::shared_ptr<ApplicationModel>& applicationModel
    ) {
        _applicationModel = applicationModel;
        ResetCamera();
    }
public:
    void Clear() const {
        _applicationModel->SimulationBuffer->Clear();
    }

    void Exit() const {
        _applicationModel->Running = false;
    }

    void ResetCamera() const {
        _applicationModel->CameraModel->offset = {0, 50};
        _applicationModel->CameraModel->zoom = 15.f;
        _applicationModel->CameraModel->target = {};
    }
};

}
