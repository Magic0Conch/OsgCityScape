#include "Render/LowRender/RTTCamera.h"
#include "Resources/Material.h"
#include "osg/Geometry"
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Program>
#include <osg/Texture2D>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <ostream>
#include <string>

using namespace CSEditor::Render;

RTTCamera::RTTCamera(RenderTexture* source,RenderTexture* destination,const std::string& vertPath,const std::string& fragPath):
m_sourceTexture(source),m_destinationTexture(destination),Material(vertPath,fragPath){
    m_camera = new osg::Camera();
    initializeRTTCameraParameters();
    m_sourceGeode = new osg::Geode;
    osg::ref_ptr<osg::Geometry> quad = osg::createTexturedQuadGeometry(osg::Vec3(-1,-1,0), osg::Vec3(2,0,0), osg::Vec3(0,2,0));
    quad->setVertexAttribArray(0,quad->getVertexArray());
    quad->setVertexAttribArray(1,quad->getTexCoordArray(0));
    m_sourceGeode->addDrawable(quad);

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



        // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // //use multi sample fbo first
        // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glEnable(GL_DEPTH_TEST);

        // shader->use();
        // shader->setValue("model", glm::mat4(1.0f));
        
        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // //copy multisample fbo to normal fbo(shader can't read from multisample fbo)
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        // glBlitFramebuffer(0, 0, EngineWindow::getInstance().viewportWidth, EngineWindow::getInstance().viewportHeight, 0, 0, EngineWindow::getInstance().viewportWidth, EngineWindow::getInstance().viewportHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glDisable(GL_DEPTH_TEST);
        
        // // draw Screen quad
        // screenShader->use();
        // glBindVertexArray(quadVAO);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, screenTexture); // use the now resolved color attachment as the quad's texture
        // glDrawArrays(GL_TRIANGLES, 0, 6);


        // glBindVertexArray(0);

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
    bind(m_sourceGeode);
    if(m_sourceTexture){
        m_stateSet->setTextureAttributeAndModes(0,m_sourceTexture.get(),
        osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);
    }
    m_stateSet->addUniform(new osg::Uniform("_MainTex",0));
    
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

void RTTCamera::addChildToRTTCamera(osg::Group* rhs){
    m_camera->addChild(rhs);
}
osg::ref_ptr<osg::StateSet> RTTCamera::getDestinationQuadStateSet() const{
    return getDestinationQuadGeode()->getStateSet();
}