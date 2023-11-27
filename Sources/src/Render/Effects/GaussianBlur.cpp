#include "Render/Effects/GaussianBlur.h"
#include "Render/LowRender/RTTCamera.h"
#include "osg/ref_ptr"
using namespace CSEditor::Render;
void GaussianBlur::generateRenderPasses(){
    osg::ref_ptr<RenderTexture> buffer0 = new RenderTexture(m_width,m_height);
    osg::ref_ptr<RenderTexture> buffer1 = new RenderTexture(m_width,m_height);
    addRenderPass(new RTTCamera(m_sourceTexture,buffer0,"resources/shaders/screen.vert", "resources/shaders/screen.frag"));
    const auto texelSize = new osg::Uniform("_MainTex_TexelSize",osg::Vec2f(1./m_width,1./m_height));
    for (int i = 0; i<m_blurIterations; i++) {
        //vertical pass
        osg::ref_ptr<RTTCamera> verticalPass = new RTTCamera(buffer0,buffer1,
        "resources/shaders/gaussianBlurVertical.vert", "resources/shaders/gaussianBlur.frag");
        verticalPass->getMaterial()->addUniform("_BlurSize",1.f+i*m_blurSpeed);
        verticalPass->getMaterial()->addUniform(texelSize);
        addRenderPass(verticalPass);
        //horizontal pass
        osg::ref_ptr<RTTCamera> horizontalPass = new RTTCamera(buffer1,buffer0,
        "resources/shaders/gaussianBlurHorizontal.vert", "resources/shaders/gaussianBlur.frag");
        horizontalPass->getMaterial()->addUniform("_BlurSize",1.f+i*m_blurSpeed);
        horizontalPass->getMaterial()->addUniform(texelSize);
        addRenderPass(horizontalPass);
    }
    addRenderPass(new RTTCamera(buffer0,m_destinationTexture,"resources/shaders/screen.vert", "resources/shaders/screen.frag"));
}

GaussianBlur::GaussianBlur(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int width,int height,int blurIterations,float blurSpeed):
m_sourceTexture(sourceTexture),m_destinationTexture(destinationTexture),m_width(width),m_height(height),m_blurIterations(){
    m_blurIterations = blurIterations;
    m_blurSpeed = blurSpeed;
    generateRenderPasses();
}

GaussianBlur::GaussianBlur(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int blurIterations,float blurSpeed):
m_sourceTexture(sourceTexture),m_destinationTexture(destinationTexture),m_width(sourceTexture->getTextureWidth()),m_height(sourceTexture->getTextureHeight()){
    m_blurIterations = blurIterations;
    m_blurSpeed = blurSpeed;
    generateRenderPasses();
}

osg::ref_ptr<RenderTexture> GaussianBlur::getDestinationTexture() const{
    return m_destinationTexture;
}

