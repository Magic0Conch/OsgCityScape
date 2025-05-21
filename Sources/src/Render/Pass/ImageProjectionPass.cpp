#include <osg/Camera>
#include <osg/Program>
#include "GL/glcorearb.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Render/Pass/ImageProjectionPass.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include "Resources/Loaders/ShaderLoader.h"
#include <osg/CullFace>
#include "osg/Image"
#include "osg/StateAttribute"
#include "osg/Texture2DArray"
#include "osg/ref_ptr"
#include "osgDB/WriteFile"

using namespace CSEditor::Render;
using namespace CSEditor;

void ImageProjectionPass::createTextureProjectionShader(const std::string& shaderName) {
    //UVUnwrap TextureProjection
    auto ss = getOrCreateStateSet();
    const std::string& vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / fmt::format("common/{}.vert", shaderName)).string();
    const std::string& fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / fmt::format("common/{}.frag", shaderName)).string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);   
    
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

ImageProjectionPass::ImageProjectionPass() {
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    setName("TextureProjection");
    auto ss = getStateSet();
    createTextureProjectionShader("TextureProjection");

    // _texture = new osg::Texture2D();
    // _texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
    // _texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    // _texture->setInternalFormat(GL_DEPTH_COMPONENT);
    // _texture->setSourceFormat(GL_DEPTH_COMPONENT);
    // _texture->setSourceType(GL_FLOAT);

    ss->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
    auto colorMask = new osg::ColorMask;
    colorMask->setMask(false, false, false, false);
    ss->setAttributeAndModes(colorMask, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    auto cullFace = new osg::CullFace;
    cullFace->setMode(osg::CullFace::BACK);
    ss->setAttributeAndModes(cullFace, osg::StateAttribute::ON);    
}

void ImageProjectionPass::setup(osg::ref_ptr<osg::GraphicsContext> gc,const int width,const int height,osg::ref_ptr<osg::Texture2D> colorTexture,osg::ref_ptr<osg::Texture2DArray> depthMap,
    osg::Matrixd projectorProjectionMatrix,osg::ref_ptr<osg::Texture2D> targetDepthStencilTexture,osg::ref_ptr<osg::Texture2D> targetTexture,unsigned int cullMask,int renderOrder){
    setGraphicsContext(gc);
    setViewport(0,0,width,height);

    _texture = colorTexture;
    _targetDepthStencilTexture = targetDepthStencilTexture;    
    _targetTexture = targetTexture;
    auto stateSet = getOrCreateStateSet();
    
    stateSet->addUniform(new osg::Uniform("depthMap", 1));
    stateSet->addUniform(new osg::Uniform("colorMap", 2));
    stateSet->addUniform(new osg::Uniform("mainTexture", 0));
    stateSet->addUniform(new osg::Uniform("mapSize", 0));
    m_lightSpaceMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "lightSpaceMatrix", 1);
    stateSet->addUniform(m_lightSpaceMatrixUniform);
    stateSet->addUniform(projectionUniform);

    m_enableProjectionUniform = new osg::Uniform(osg::Uniform::BOOL, "_EnableProjection", 1);
    stateSet->addUniform(m_enableProjectionUniform);

    setNodeMask(0x1);
    setCullingMode(getCullingMode() & ~osg::CullSettings::SMALL_FEATURE_CULLING);
    attach(osg::Camera::COLOR_BUFFER0, targetTexture);
    attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER, _targetDepthStencilTexture);

    auto cullFace = new osg::CullFace;
    cullFace->setMode(osg::CullFace::BACK);
    stateSet->setAttributeAndModes(cullFace, osg::StateAttribute::ON);

    setCullMask(cullMask);
    setRenderOrder(osg::Camera::PRE_RENDER, renderOrder);

    
    m_colorMap = new osg::Texture2DArray;
    m_colorMap->setInternalFormat(GL_RGBA);    
    m_colorMap->setTextureSize(colorTexture->getTextureWidth(), colorTexture->getTextureHeight(), 1);
    m_colorMap->setInternalFormat(colorTexture->getInternalFormat());
    m_colorMap->setImage(0, colorTexture->getImage());

    stateSet->setTextureAttributeAndModes(1, depthMap, osg::StateAttribute::ON);
    stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
    stateSet->getUniform("mapSize")->set(1);
    
    m_lightSpaceMatrixUniform->setElement(0, projectorProjectionMatrix);
    m_enableProjectionUniform->setElement(0, true);
    
}


osg::ref_ptr<osg::Texture2D> ImageProjectionPass::getColorTexture() {
    return _texture; 
}
osg::ref_ptr<osg::Texture2D> ImageProjectionPass::getDepthStencilTexture() { 
    return _targetDepthStencilTexture; 
}

void ImageProjectionPass::setProjectionMatrix(const osg::Matrixd& projectionMatrix) {
    projectionUniform->set(projectionMatrix);
}

void ImageProjectionPass::setLightSpaceMatrixUniform(int index,osg::Matrixd& lightSpaceMatrix){
    auto ss = getOrCreateStateSet();        
    auto projectionMatrixUniform = ss->getUniform("lightSpaceMatrix");
    if (!projectionMatrixUniform) {
        // 创建并添加 uniform 如果不存在
        projectionMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "lightSpaceMatrix");
        ss->addUniform(projectionMatrixUniform);
    }
    projectionMatrixUniform->setElement(index, lightSpaceMatrix);
}

void ImageProjectionPass::setProjectionEnabled(int index, bool enable){
    m_enableProjectionUniform->setElement(index, enable);
}



// void TextureProjectionPass::setTextureArray(osg::ref_ptr<osg::Texture2DArray> depthMapArray, std::vector<osg::ref_ptr<osg::Texture2D>> colorTexVec, std::vector<osg::Matrixd>& projectorProjectionMatrix) {
//     if(depthMapArray == nullptr || colorTexVec.empty() || colorTexVec[0] == nullptr || projectionMatrixVec.empty()) {
//         return;
//     }
//     if (!m_colorMap) {
//         m_colorMap = new osg::Texture2DArray;
//         m_colorMap->setInternalFormat(GL_RGBA);
//     }
//     int cnt = colorTexVec.size();
//     m_colorMap->setTextureSize(colorTexVec[0]->getTextureWidth(), colorTexVec[0]->getTextureHeight(), cnt);
//     m_colorMap->setInternalFormat(colorTexVec[0]->getInternalFormat());
//     // m_colorMap->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION);
//     // m_colorMap->setUnRefImageDataAfterApply(true);
//     for (int i = 0; i < cnt; i++) {
//         m_colorMap->setImage(i, colorTexVec[i]->getImage());
//     }
//     auto stateSet = m_camera->getOrCreateStateSet();
//     stateSet->setTextureAttributeAndModes(1, depthMapArray, osg::StateAttribute::ON);
//     stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
//     stateSet->getUniform("mapSize")->set(cnt);
//     for (int i = 0; i < cnt; ++i) {
//         auto& projectionMatrix = projectionMatrixVec[i];
//         m_lightSpaceMatrixUniform->setElement(i, projectionMatrix);
//         m_enableProjectionUniform->setElement(i, true);
//     }
// }


// void ImageProjectionPass::setTexture(int index ,osg::ref_ptr<osg::Texture2D> tex){
//     auto stateSet = m_camera->getOrCreateStateSet();
//     osg::Image* image = tex->getImage();
//     m_colorMap->setImage(index, image);
//     stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
// }

// void ImageProjectionPass::setTexture(int index ,osg::ref_ptr<osg::Image> img){
//     auto stateSet = m_camera->getOrCreateStateSet();
//     m_colorMap->setImage(index, img);
//     stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
// }
