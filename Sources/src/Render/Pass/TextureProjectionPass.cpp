#include <osg/Camera>
#include <osg/Program>
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include "Resources/Loaders/ShaderLoader.h"
#include "osg/Image"

using namespace CSEditor::Render;
using namespace CSEditor;
void createTextureProjectionShader(osg::StateSet* ss) {
    const std::string& vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/TextureProjection.vert").string();
    const std::string& fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/TextureProjection.frag").string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);   
    
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

TextureProjectionPass::TextureProjectionPass(osg::ref_ptr<osg::Camera> camera):m_camera(camera) {
    m_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    m_camera->setRenderOrder(osg::Camera::PRE_RENDER);
    m_camera->setName("RenderColor");

    _texture = new osg::Texture2D();
    _texture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
    _texture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
    _texture->setSourceFormat(GL_RGBA);
    _texture->setInternalFormat(GL_RGBA8);
    _texture->setSourceType(GL_FLOAT);

    _depthStencilTexture = new osg::Texture2D();
    _depthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
    _depthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
    _depthStencilTexture->setSourceFormat(GL_DEPTH_STENCIL);
    _depthStencilTexture->setInternalFormat(GL_DEPTH_STENCIL);
    _depthStencilTexture->setSourceType(GL_UNSIGNED_INT_24_8);    

    auto stateSet = m_camera->getOrCreateStateSet();
    m_camera->setRenderOrder(osg::Camera::POST_RENDER);
    createTextureProjectionShader(stateSet);
    stateSet->addUniform(new osg::Uniform("depthMap", 1));
    stateSet->addUniform(new osg::Uniform("colorMap", 2));
    stateSet->addUniform(new osg::Uniform("mainTexture", 0));
    stateSet->addUniform(new osg::Uniform("mapSize", 0));
    m_lightSpaceMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "lightSpaceMatrix", 16);
    stateSet->addUniform(m_lightSpaceMatrixUniform);

    m_camera->attach(osg::Camera::COLOR_BUFFER0, _texture);
    
    m_camera->attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER, _depthStencilTexture);
    Core::g_runtimeContext.windowSystem->setScreenTexture(_texture);
}


void TextureProjectionPass::setTextureArray(osg::ref_ptr<osg::Texture2DArray> depthMapArray, std::vector<osg::Texture2D *> colorTexVec, std::vector<osg::Matrixd*> projectionMatrixVec) {
    if (!m_colorMap) {
        m_colorMap = new osg::Texture2DArray;
        m_colorMap->setInternalFormat(GL_RGBA);
    }
    int cnt = colorTexVec.size();
    m_colorMap->setTextureSize(colorTexVec[0]->getTextureWidth(), colorTexVec[0]->getTextureHeight(), cnt);
    m_colorMap->setInternalFormat(colorTexVec[0]->getInternalFormat());

    for (int i = 0; i < cnt; i++) {
        m_colorMap->setImage(i, colorTexVec[i]->getImage());
    }
    auto stateSet = m_camera->getOrCreateStateSet();
    stateSet->setTextureAttributeAndModes(1, depthMapArray, osg::StateAttribute::ON);
    stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
    stateSet->getUniform("mapSize")->set(cnt);
    for (int i = 0; i < cnt; ++i) {
        auto& projectionMatrix = *projectionMatrixVec[i];
        m_lightSpaceMatrixUniform->setElement(i, projectionMatrix);
    }
}


void TextureProjectionPass::setTexture(osg::Texture2D * tex) {
    auto stateSet = m_camera->getOrCreateStateSet();
    stateSet->setTextureAttributeAndModes(2, tex, osg::StateAttribute::ON);
}
