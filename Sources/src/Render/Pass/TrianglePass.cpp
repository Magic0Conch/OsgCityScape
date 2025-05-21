#include "Render/Pass/TrianglePass.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Resources/Loaders/ShaderLoader.h"
#include <osg/StateSet>
#include <osg/Uniform>
#include <osg/Program>

using namespace CSEditor::Render;

TrianglePass::TrianglePass()
{
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    setName("TrianglePass");
    auto ss = getStateSet();
    createTriangleShader(ss);

    _projectionUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "_ProjectionMatrix");
    ss->addUniform(_projectionUniform);
}

TrianglePass::~TrianglePass() {}

void TrianglePass::createTriangleShader(osg::StateSet* ss)
{
    // 路径请按你的实际工程结构调整
    auto vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.vert").string();
    auto fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.frag").string();

    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

void TrianglePass::setup(
    osg::ref_ptr<osg::GraphicsContext> gc,
    int width,
    int height,
    osg::ref_ptr<osg::Texture2D> inputTexture,
    unsigned int cullMask,
    int renderOrder
) {
    setGraphicsContext(gc);
    setViewport(0, 0, width, height);
    setCullMask(cullMask);
    setRenderOrder(osg::Camera::PRE_RENDER, renderOrder);

    _inputTexture = inputTexture;
    auto ss = getOrCreateStateSet();
    ss->setTextureAttributeAndModes(0, _inputTexture, osg::StateAttribute::ON);
    ss->addUniform(new osg::Uniform("mainTexture", 0));
}

void TrianglePass::setProjectionMatrix(const osg::Matrixd& proj)
{
    _projectionUniform->set(proj);
}
