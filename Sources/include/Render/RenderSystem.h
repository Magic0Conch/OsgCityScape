#pragma once
#include "Pass/OpaqueEffectPass.h"
#include "Render/Pass/DepthPass.h"
#include "Render/Pass/ImageProjectionPass.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Resources/ResourceType/Common/Level.h"
#include "Render/Debugging/CaptureCallback.h"
#include "Render/Pass/ObjectPickerPass.h"
#include "Render/Config/RenderConfig.h"
#include "osg/Camera"
#include "osg/ref_ptr"
#include <memory>

namespace CSEditor::Render {
class RenderSystem{
public:
    RenderSystem();
    void projectionCameraChanged();
    void createLightMatrices();
    void createResources();
    void setupRenderPasses();
    void initialize();
    void tick(float deltaTime);
    osg::Matrixd& getMainProjectionMatrix();
    void captureFrame();
    void setRenderPipelineState(const RenderPipelineState renderPipelineState);
    std::shared_ptr<int> getCaptureFlag();
    void deleteSlaveCameras();
private:
    const int width = 1080;
    const int height = 720;
    std::shared_ptr<ResourceType::Level> m_levelResource;
    std::shared_ptr<ECS::Level> m_level;
    std::vector<osg::ref_ptr<osg::Texture2DArray>> m_depthArray;
    std::vector<osg::Matrixd> m_lightMatrices;
    std::shared_ptr<osg::Matrixd>  m_mainProjectionMatrix;
    std::vector<Render::DepthPass> m_depthPassVector;
    std::vector<osg::ref_ptr<osg::Texture2D>> m_colorTextureVector;
    osg::ref_ptr<osg::Camera> m_mainCamera;
    osg::ref_ptr<osg::Texture2D> m_mainColorTexture;
    osg::ref_ptr<osg::Texture2D> m_mainDepthStencilTexture;
    osg::ref_ptr<osg::Texture2D> m_projectorColorTexture;
    osg::ref_ptr<osg::Texture2D> m_projectorDepthStencilTexture;
    osg::ref_ptr<osg::GraphicsContext> m_graphicsContext;
    osg::ref_ptr<CaptureCallback> m_captureCallback;
    std::shared_ptr<int> m_captureFlag = std::make_shared<int>(3);
    //Passes
    std::vector<osg::ref_ptr<Render::DepthPass>> m_depthPasses;
    std::vector<osg::ref_ptr<ImageProjectionPass>> m_imageProjectionPasses;
    
    std::shared_ptr<Render::TextureProjectionPass> m_textureProjectionPass;
    osg::ref_ptr<Render::OpaqueEffectPass> m_opaqueEffectPass;
    std::unique_ptr<ObjectPickerPass> m_objectPickerPass;
};
}