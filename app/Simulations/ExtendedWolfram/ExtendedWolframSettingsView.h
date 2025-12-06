#pragma once
#include <memory>

#include "./ExtendedWolframSimulationBuffer.h"

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
        FieldScalar("Neighbours", "##neighbours", ImGuiDataType_U32, &_config->Neighbors);
        FieldScalar("Rule", "##rule", ImGuiDataType_U32, &_config->Rule);
        FieldScalar("Width", "##width", ImGuiDataType_U32, &_config->Width);
        FieldBool("Is Looped", "##isloop", &_config->IsLoop);
    }
};

}
