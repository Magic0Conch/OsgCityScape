#include "Render/Pass/DepthPass.h"
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

using namespace CSEditor::Render; 

void DepthPass::setup(osg::ref_ptr<osg::GraphicsContext> gc,const int width,const int height,
osg::ref_ptr<osg::Texture2DArray> depthArray,int depthTextureIndex,unsigned int cullMask,int renderOrder){
    osg::ref_ptr<osg::Texture2D> dummyColor = new osg::Texture2D();
    dummyColor->setTextureSize(width, height); // 或width, height
    dummyColor->setInternalFormat(GL_RGBA);
    dummyColor->setSourceFormat(GL_RGBA);
    dummyColor->setSourceType(GL_UNSIGNED_BYTE);
    dummyColor->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
    dummyColor->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);

    setGraphicsContext(gc);
    setViewport(0,0,width,height);
    attach(osg::Camera::DEPTH_BUFFER,depthArray.get());
    attach(osg::Camera::COLOR_BUFFER0, dummyColor.get());
    setCullMask(0x1);
    setRenderOrder(osg::Camera::PRE_RENDER, 0);
    setName("DepthPass");
    

}

void DepthPass::createDepthShader(osg::StateSet* ss) {
    const std::string& vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/Depth.vert").string();
    const std::string& fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/Depth.frag").string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);        
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}


DepthPass::DepthPass()
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

osg::Texture2D* DepthPass::getTexture() const
{
    return _texture.get();
}

void DepthPass::setViewMatrix(const osg::Matrixd& viewMatrix){
    auto ss = getOrCreateStateSet();
    osg::ref_ptr<osg::Uniform> viewMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"_ViewMatrix");
    if (viewMatrixUniform) {
        viewMatrixUniform->set(viewMatrix);
    } else {
        viewMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "_ViewMatrix");
        viewMatrixUniform->set(viewMatrix);
        ss->addUniform(viewMatrixUniform);
    }
    viewMatrixUniform->set(viewMatrix);
    ss->addUniform(viewMatrixUniform);
}

void DepthPass::setProjectionMatrix(const osg::Matrixd& projectionMatrix){
    auto ss = getOrCreateStateSet();
    osg::ref_ptr<osg::Uniform> projectionMatrixUniform = ss->getUniform("_ProjectionMatrix");
    if (projectionMatrixUniform) {
        projectionMatrixUniform->set(projectionMatrix);
    } else {
        projectionMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "_ProjectionMatrix");
        projectionMatrixUniform->set(projectionMatrix);
        ss->addUniform(projectionMatrixUniform);
    }
}

DepthPass::~DepthPass(){

}



// void RenderDepthToTexture::setViewProjectionMatrix(const osg::Matrixd& viewMatrix){
//     auto ss = getOrCreateStateSet();
//     osg::ref_ptr<osg::Uniform> ViewProjectionMatrixUniform = new osg::Uniform("osg_ModelViewProjectionMatrix", viewMatrix);
//     ss->addUniform(ViewProjectionMatrixUniform);
// }

