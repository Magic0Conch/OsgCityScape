#include "Render/Pass/TextureDisplayPass.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/Program>
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Resources/Loaders/ShaderLoader.h"
#include "Windowing/Window.h"

using namespace CSEditor::Render;

// 生成一个 [-1, 1] 到 [1, -1] 的全屏quad，带有0-1的UV
namespace {
    osg::ref_ptr<osg::Geode> createFullscreenQuad() {
        osg::ref_ptr<osg::Geometry> quad = osg::createTexturedQuadGeometry(
            osg::Vec3(-1.0f, -1.0f, 0.0f),   // left-bottom
            osg::Vec3(2.0f, 0.0f, 0.0f),     // x-width
            osg::Vec3(0.0f, 2.0f, 0.0f),     // y-height
            0.0f, 0.0f, 1.0f, 1.0f           // uv
        );
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        geode->addDrawable(quad.get());
        geode->setNodeMask(0x100);
        // quad->setNodeMask(0x100);
        return geode;
    }
}

TextureDisplayPass::TextureDisplayPass()
{
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT); // 渲染到FBO（或者窗口）
    setName("TextureDisplayPass");
    setReferenceFrame(osg::Camera::ABSOLUTE_RF);
    setAllowEventFocus(false); // 禁止事件响应
}

void TextureDisplayPass::setup(osg::ref_ptr<osg::GraphicsContext> gc,
                               int width, int height,
                               osg::ref_ptr<osg::Texture2D> inputTexture,
                               int renderOrder)
{
    setGraphicsContext(gc);
    setViewport(0, 0, width, height);
    setRenderOrder(osg::Camera::POST_RENDER, renderOrder);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setProjectionMatrixAsOrtho2D(-1, 1, -1, 1);
    setCullMask(0x100);
    // 1. 清空之前内容（防止多次setup造成重复add）
    if (_fullscreenQuad) {
        removeChild(_fullscreenQuad);
        _fullscreenQuad = nullptr;
    }

    // 2. 创建全屏quad，只添加自己
    _fullscreenQuad = createFullscreenQuad();
    addChild(_fullscreenQuad.get());

    // 3. 绑定shader和纹理
    osg::StateSet* ss = _fullscreenQuad->getOrCreateStateSet();
    // 默认shader
    auto vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/texture_display.vert").string();
    auto fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/texture_display.frag").string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

    // 输入纹理
    ss->setTextureAttributeAndModes(0, inputTexture, osg::StateAttribute::ON);

    if (!_mainTextureUniform) {
        _mainTextureUniform = new osg::Uniform("mainTexture", 0);
        ss->addUniform(_mainTextureUniform);
    }

    Core::g_runtimeContext.windowSystem->setScreenTexture(inputTexture);
}

void TextureDisplayPass::setShader(const std::string& vertPath, const std::string& fragPath)
{
    if (!_fullscreenQuad) return;
    osg::StateSet* ss = _fullscreenQuad->getOrCreateStateSet();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}
