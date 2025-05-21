#include "Render/Pass/TrianglePass.h"
#include "Editor/Core/RuntimeContext.h"
#include "GL/glcorearb.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Resources/Loaders/ShaderLoader.h"
#include <osg/StateSet>
#include <osg/Uniform>
#include <osg/Program>

using namespace CSEditor::Render;

TrianglePass::TrianglePass()
{
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT) ;
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    setName("TrianglePass");
    auto ss = getStateSet();
    // 绑定shader
    auto vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.vert").string();
    auto fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.frag").string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

    _projectionUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "_ProjectionMatrix");
    ss->addUniform(_projectionUniform);
}

TrianglePass::~TrianglePass() {}

void TrianglePass::setup(
    osg::ref_ptr<osg::GraphicsContext> gc,
    int width,
    int height,
    osg::ref_ptr<osg::Texture2D> renderTargetTexture,
    osg::ref_ptr<osg::Texture2D> depthStencilTexture,  // 新增参数,
    unsigned int cullMask,
    int renderOrder
) {
    setGraphicsContext(gc);
    setViewport(0, 0, width, height);
    setCullMask(cullMask);
    setRenderOrder(osg::Camera::PRE_RENDER, renderOrder);

    // 输出到renderTargetTexture
    attach(osg::Camera::COLOR_BUFFER0, renderTargetTexture);

    if (depthStencilTexture) {
        attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER, depthStencilTexture);
    }
    // 输入贴图绑定到0槽
    auto ss = getOrCreateStateSet();
    ss->addUniform(new osg::Uniform("mainTexture", 0));
}

void TrianglePass::setProjectionMatrix(const osg::Matrixd& proj)
{
    _projectionUniform->set(proj);
}
