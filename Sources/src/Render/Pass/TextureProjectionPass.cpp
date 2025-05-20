#include <osg/Camera>
#include <osg/Program>
#include "GL/glcorearb.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include "Resources/Loaders/ShaderLoader.h"
#include <osg/CullFace>
#include "osg/Image"
#include "osg/StateAttribute"
#include "osg/ref_ptr"
#include "osgDB/WriteFile"

using namespace CSEditor::Render;
using namespace CSEditor;

void TextureProjectionPass::createTextureProjectionShader(const std::string& shaderName) {
    //UVUnwrap TextureProjection
    auto ss = m_camera->getOrCreateStateSet();
    const std::string& vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / fmt::format("common/{}.vert", shaderName)).string();
    const std::string& fragPath = (Core::g_runtimeContext.configManager->getShaderFolder() / fmt::format("common/{}.frag", shaderName)).string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath, fragPath);   
    
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

TextureProjectionPass::TextureProjectionPass() {
}

void TextureProjectionPass::setup(osg::ref_ptr<osg::Camera> camera,osg::ref_ptr<osg::Texture2D> colorTexture,
    osg::ref_ptr<osg::Texture2D> depthStencilTexture,unsigned int cullMask,int renderOrder){
    m_camera = camera;
    m_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    m_camera->setName("RenderColor");

    _texture = colorTexture;
    _depthStencilTexture = depthStencilTexture;    

    auto stateSet = m_camera->getOrCreateStateSet();
    createTextureProjectionShader("TextureProjection");
    stateSet->addUniform(new osg::Uniform("depthMap", 1));
    stateSet->addUniform(new osg::Uniform("colorMap", 2));
    stateSet->addUniform(new osg::Uniform("mainTexture", 0));
    stateSet->addUniform(new osg::Uniform("mapSize", 0));
    m_lightSpaceMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "lightSpaceMatrix", 16);
    stateSet->addUniform(m_lightSpaceMatrixUniform);
    stateSet->addUniform(projectionUniform);

    m_enableProjectionUniform = new osg::Uniform(osg::Uniform::BOOL, "_EnableProjection", 16);
    stateSet->addUniform(m_enableProjectionUniform);

    // m_camera->setNodeMask(0x1);
    m_camera->setCullingMode(m_camera->getCullingMode() & ~osg::CullSettings::SMALL_FEATURE_CULLING);
    m_camera->attach(osg::Camera::COLOR_BUFFER0, _texture);
    m_camera->attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER, _depthStencilTexture);
    Core::g_runtimeContext.windowSystem->setScreenTexture(_texture);
    auto cullFace = new osg::CullFace;
    cullFace->setMode(osg::CullFace::BACK);
    stateSet->setAttributeAndModes(cullFace, osg::StateAttribute::ON);

    m_camera->setCullMask(cullMask);
    m_camera->setRenderOrder(osg::Camera::PRE_RENDER, renderOrder);
}



void TextureProjectionPass::setTextureArray(osg::ref_ptr<osg::Texture2DArray> depthMapArray, std::vector<osg::ref_ptr<osg::Texture2D>> colorTexVec, std::vector<osg::Matrixd>& projectionMatrixVec) {
    if(depthMapArray == nullptr || colorTexVec.empty() || colorTexVec[0] == nullptr || projectionMatrixVec.empty()) {
        return;
    }
    if (!m_colorMap) {
        m_colorMap = new osg::Texture2DArray;
        m_colorMap->setInternalFormat(GL_RGBA);
    }
    int cnt = colorTexVec.size();
    m_colorMap->setTextureSize(colorTexVec[0]->getTextureWidth(), colorTexVec[0]->getTextureHeight(), cnt);
    m_colorMap->setInternalFormat(colorTexVec[0]->getInternalFormat());
    // m_colorMap->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION);
    // m_colorMap->setUnRefImageDataAfterApply(true);
    for (int i = 0; i < cnt; i++) {
        m_colorMap->setImage(i, colorTexVec[i]->getImage());
    }
    auto stateSet = m_camera->getOrCreateStateSet();
    stateSet->setTextureAttributeAndModes(1, depthMapArray, osg::StateAttribute::ON);
    stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
    stateSet->getUniform("mapSize")->set(cnt);
    for (int i = 0; i < cnt; ++i) {
        auto& projectionMatrix = projectionMatrixVec[i];
        m_lightSpaceMatrixUniform->setElement(i, projectionMatrix);
        m_enableProjectionUniform->setElement(i, true);
    }
}


void TextureProjectionPass::setTexture(int index ,osg::ref_ptr<osg::Texture2D> tex){
    auto stateSet = m_camera->getOrCreateStateSet();
    osg::Image* image = tex->getImage();
    m_colorMap->setImage(index, image);
    stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
}

void TextureProjectionPass::setTexture(int index ,osg::ref_ptr<osg::Image> img){
    auto stateSet = m_camera->getOrCreateStateSet();
    m_colorMap->setImage(index, img);
    stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
}

osg::ref_ptr<osg::Texture2D> TextureProjectionPass::getColorTexture() {
    return _texture; 
}
osg::ref_ptr<osg::Texture2D> TextureProjectionPass::getDepthStencilTexture() { 
    return _depthStencilTexture; 
}

void TextureProjectionPass::setProjectionMatrix(const osg::Matrixd& projectionMatrix) {
    projectionUniform->set(projectionMatrix);
}

void TextureProjectionPass::setLightSpaceMatrixUniform(int index,osg::Matrixd& lightSpaceMatrix){
    auto ss = m_camera->getOrCreateStateSet();        
    auto projectionMatrixUniform = ss->getUniform("lightSpaceMatrix");
    if (!projectionMatrixUniform) {
        // 创建并添加 uniform 如果不存在
        projectionMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "lightSpaceMatrix");
        ss->addUniform(projectionMatrixUniform);
    }
    projectionMatrixUniform->setElement(index, lightSpaceMatrix);
}

void TextureProjectionPass::setProjectionEnabled(int index, bool enable){
    m_enableProjectionUniform->setElement(index, enable);
}

void TextureProjectionPass::saveTexture(std::string savePath){
    osg::ref_ptr<osg::Image> image = new osg::Image;
    // image->allocateImage(4096, 4096, 1, GL_RGBA, GL_UNSIGNED_BYTE);
    image->readPixels(0, 0, 4096, 4096, GL_RGBA, GL_UNSIGNED_BYTE);
    
    osg::ref_ptr<osg::FrameBufferObject> fbo = new osg::FrameBufferObject;
    fbo->setAttachment(osg::Camera::COLOR_BUFFER, osg::FrameBufferAttachment(_texture));
    auto state = m_camera->getGraphicsContext()->getState();
    fbo->apply(*state);
    glReadPixels(0, 0,4096, 4096, GL_RGBA, GL_UNSIGNED_BYTE, image->data());
    try {
        // bool result = osgDB::writeImageFile(*outputImage, "output.jpg");
        bool result = osgDB::writeImageFile(*image,"output.jpg");
        if (!result) {
            std::cerr << "Error writing file output.jpg" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    _texture->apply(*state);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data());
}