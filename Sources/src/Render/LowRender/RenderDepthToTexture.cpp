#include "Render/LowRender/RenderDepthToTexture.h"
#include "Editor/Core/RuntimeContext.h"
#include "osg/StateSet"
#include <string>
#include <osg/Shader>
#include <osg/Program>
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Resources/Loaders/ShaderLoader.h"
#include "osg/Uniform"

namespace CSEditor::Render {


void createDepthShader(osg::StateSet* ss) {
    const std::string& vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/Depth.vert").string();
    const std::string& fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/Depth.frag").string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);        
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}


RenderDepthToTexture::RenderDepthToTexture()
{
    setClearMask(GL_DEPTH_BUFFER_BIT);
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    setRenderOrder(osg::Camera::PRE_RENDER);
    setName("RenderDepth");
    auto ss = getStateSet();
    createDepthShader(ss);
    _texture = new osg::Texture2D();
    _texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
    _texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    _texture->setInternalFormat(GL_DEPTH_COMPONENT);
    _texture->setSourceFormat(GL_DEPTH_COMPONENT);
    _texture->setSourceType(GL_FLOAT);
    // attach(osg::Camera::DEPTH_BUFFER, _texture);
    // createDepthVisualizationShader(0.1, 1000, getOrCreateStateSet());
}

osg::Texture2D* RenderDepthToTexture::getTexture() const
{
    return _texture.get();
}

void RenderDepthToTexture::setViewProjectionMatrix(const osg::Matrixd& viewMatrix){
    auto ss = getOrCreateStateSet();
    osg::ref_ptr<osg::Uniform> viewProjectionMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"viewProjectionMatrix");
    viewProjectionMatrixUniform->set(viewMatrix);
    ss->addUniform(viewProjectionMatrixUniform);
}

}

// void RenderDepthToTexture::setViewProjectionMatrix(const osg::Matrixd& viewMatrix){
//     auto ss = getOrCreateStateSet();
//     osg::ref_ptr<osg::Uniform> ViewProjectionMatrixUniform = new osg::Uniform("osg_ModelViewProjectionMatrix", viewMatrix);
//     ss->addUniform(ViewProjectionMatrixUniform);
// }

