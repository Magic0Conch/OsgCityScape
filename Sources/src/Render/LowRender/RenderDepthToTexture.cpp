#include "Render/LowRender/RenderDepthToTexture.h"
#include "Editor/Core/RuntimeContext.h"
#include "osg/CullFace"
#include "osg/StateAttribute"
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
    setName("RenderDepth");
    auto ss = getStateSet();
    createDepthShader(ss);
    _texture = new osg::Texture2D();
    _texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
    _texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    _texture->setInternalFormat(GL_DEPTH_COMPONENT);
    _texture->setSourceFormat(GL_DEPTH_COMPONENT);
    _texture->setSourceType(GL_FLOAT);

    ss->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
    auto colorMask = new osg::ColorMask;
    colorMask->setMask(false, false, false, false);
    ss->setAttributeAndModes(colorMask, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    auto cullFace = new osg::CullFace;
    cullFace->setMode(osg::CullFace::BACK);
    ss->setAttributeAndModes(cullFace, osg::StateAttribute::ON);
}

osg::Texture2D* RenderDepthToTexture::getTexture() const
{
    return _texture.get();
}

void RenderDepthToTexture::setViewProjectionMatrix(const osg::Matrixd& modelViewProjectionMatrix){
    auto ss = getOrCreateStateSet();
    osg::ref_ptr<osg::Uniform> viewProjectionMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"modelViewProjectionMatrix");
    viewProjectionMatrixUniform->set(modelViewProjectionMatrix);
    ss->addUniform(viewProjectionMatrixUniform);
}

}

// void RenderDepthToTexture::setViewProjectionMatrix(const osg::Matrixd& viewMatrix){
//     auto ss = getOrCreateStateSet();
//     osg::ref_ptr<osg::Uniform> ViewProjectionMatrixUniform = new osg::Uniform("osg_ModelViewProjectionMatrix", viewMatrix);
//     ss->addUniform(ViewProjectionMatrixUniform);
// }

