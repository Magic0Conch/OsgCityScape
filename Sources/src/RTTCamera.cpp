#include "RTTCamera.h"
#include "Utils/ShaderUtils.h"
#include "BaseGeometryFactory/GeometryFactory.h"
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
m_sourceTexture(source),m_destinationTexture(destination),
m_vertShaderPath(vertPath),m_fragShaderPath(fragPath){
    initialize();
    m_quadGeode = new osg::Geode;
    m_quadGeode->addDrawable(BaseGeometryFactory::createBaseGeometry(BaseGeometryType::QUAD));
    setDefaultUniformParameters();
    addChild(m_quadGeode);
}
RTTCamera::RTTCamera(osg::Group* source,RenderTexture* destination,const std::string& vertPath,const std::string& fragPath):
m_destinationTexture(destination),m_vertShaderPath(vertPath),m_fragShaderPath(fragPath){
    initialize();
    auto tryGeode = dynamic_cast<osg::Geode*>(source);
    if(tryGeode){
        m_quadGeode = tryGeode;
        setDefaultUniformParameters();
        addChild(m_quadGeode);
        //TODO: custom shader that processes common geometry(not a quad)
    }
    else{
        std::cout<<"ERR:Source is not a geode"<<std::endl;
    }
    
}

void RTTCamera::initialize(){
    setClearColor(osg::Vec4f(1.,0,0,0));
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    setRenderOrder(osg::Camera::PRE_RENDER);
    setRenderTargetImplementation(FRAME_BUFFER_OBJECT);
    getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //attach destination texture
    setViewport(0, 0, m_destinationTexture->getTextureWidth(), m_destinationTexture->getTextureHeight());
    attach(osg::Camera::COLOR_BUFFER, m_destinationTexture.get());
}

//set m_sourceTexture as sampler texture.
void RTTCamera::setDefaultUniformParameters(){
    ShaderUtils::setShaderProgram(m_quadGeode->getOrCreateStateSet(),m_vertShaderPath, m_fragShaderPath);
    if(m_sourceTexture){
        m_quadGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0,m_sourceTexture.get(),
        osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);
    }
    m_quadGeode->getOrCreateStateSet()->addUniform(new osg::Uniform("_MainTex",0));
    
}

osg::ref_ptr<RenderTexture> RTTCamera::getDestinationTexture() const{
    return m_destinationTexture;
}

osg::ref_ptr<osg::Geode> RTTCamera::getDestinationQuadGeode() const{
    return m_quadGeode;
}

void RTTCamera::addUniform(osg::Uniform* uniform){
    m_quadGeode->getOrCreateStateSet()->addUniform(uniform);
}