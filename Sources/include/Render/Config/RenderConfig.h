#pragma once

namespace CSEditor::Render {
enum class RenderPipelineState{
    Default,
    TextureBaking
};

class RenderConfig{
public:
    RenderConfig() = default;

    static RenderConfig& getInstance(){
        static RenderConfig renderConfig;
        return renderConfig;
    }

    RenderPipelineState renderPipelineState = RenderPipelineState::Default;
};
}