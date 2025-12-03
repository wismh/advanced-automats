#pragma once
#include <memory>

#include "ExtendedWolfram/ExtendedWolframSimulationBuffer.h"

namespace Program {

class ExtendedWolframSettingsView final : public SettingsContentView {
    std::shared_ptr<ExtendedWolframSimulationConfig> _config;
public:
    explicit ExtendedWolframSettingsView(
        const std::shared_ptr<ExtendedWolframSimulationConfig>& config
    ) :
        _config(config) {}
protected:
    void DrawContent() override {
        Field("Neighbours", "##neighbours", &_config->Neighbors);
        Field("Rule", "##rule", &_config->Rule);
    }
};

}
