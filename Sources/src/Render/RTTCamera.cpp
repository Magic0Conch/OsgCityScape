#include "Render/RTTCamera.h"
#include "Render/Material.h"
#include "Utils/ShaderUtils.h"
#include "BaseGeometryFactory/GeometryFactory.h"
#include "osg/Geometry"
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Program>
#include <osg/Texture2D>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <ostream>
#include <string>

using namespace cs;

RTTCamera::RTTCamera(RenderTexture* source,RenderTexture* destination,const std::string& vertPath,const std::string& fragPath):
m_sourceTexture(source),m_destinationTexture(destination),Material(vertPath,fragPath){
    m_camera = new osg::Camera();
    initializeRTTCameraParameters();
    m_sourceGeode = new osg::Geode;
    m_sourceGeode->addDrawable(BaseGeometryFactory::createBaseGeometry(BaseGeometryType::QUAD));
    setDefaultUniformParameters();
    m_camera->addChild(m_sourceGeode);
}
RTTCamera::RTTCamera(osg::Group* source,RenderTexture* destination,
const std::string& vertPath,const std::string& fragPath):
m_destinationTexture(destination),Material(vertPath,fragPath){
    m_camera = new osg::Camera();
    initializeRTTCameraParameters();
    auto tryGeode = dynamic_cast<osg::Geode*>(source);
    if(tryGeode){   
        m_sourceGeode = tryGeode;
        setDefaultUniformParameters();
        m_camera->addChild(m_sourceGeode);
    }
    else{
        std::cout<<"ERR:Source is not a geode"<<std::endl;
    }    
}

void RTTCamera::initializeRTTCameraParameters(){
    m_camera->setClearColor(osg::Vec4f(0,0,0,0));
    m_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // m_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    m_camera->setRenderOrder(osg::Camera::PRE_RENDER);
    m_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    m_camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //attach destination texture
    if(m_destinationTexture){
        m_camera->setViewport(0, 0, m_destinationTexture->getTextureWidth(), m_destinationTexture->getTextureHeight());
        m_camera->attach(osg::Camera::COLOR_BUFFER0, m_destinationTexture.get());
    }
}

//set m_sourceTexture as sampler texture.
void RTTCamera::setDefaultUniformParameters(){
    ShaderUtils::setShaderProgram(m_sourceGeode->getOrCreateStateSet(),m_vertShaderPath, m_fragShaderPath);
    if(m_sourceTexture){
        m_sourceGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0,m_sourceTexture.get(),
        osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);
    }
    m_sourceGeode->getOrCreateStateSet()->addUniform(new osg::Uniform("_MainTex",0));
    
}

osg::ref_ptr<RenderTexture> RTTCamera::getDestinationTexture() const{
    return m_destinationTexture;
}

osg::ref_ptr<osg::Geode> RTTCamera::getDestinationQuadGeode() const{
    return m_sourceGeode;
}
osg::ref_ptr<osg::Camera> RTTCamera::getRTTCamera() const{
    return m_camera;
}