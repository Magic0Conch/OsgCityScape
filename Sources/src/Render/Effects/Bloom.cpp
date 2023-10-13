#include "Render/Effects/Bloom.h"
using namespace CSEditor::Render;
void Bloom::generateRenderPasses(){
    osg::ref_ptr<RenderTexture> buffer0 = new RenderTexture(m_width,m_height);
    osg::ref_ptr<RenderTexture> buffer1 = new RenderTexture(m_width,m_height);
    osg::ref_ptr<RTTCamera> extractBrightnessRenderPass = new RTTCamera(m_sourceTexture,buffer0,"resources/shaders/screen.vert", "resources/shaders/bloomExtractBright.frag");
    extractBrightnessRenderPass->addUniform("_LuminanceThreshold",m_luminanceThreshold);
    addRenderPass(extractBrightnessRenderPass);
    m_gaussianBlurPipeline = new GaussianBlur(buffer0,buffer1,m_blurIterations,m_blurSpeed);
    addRenderPipeline(m_gaussianBlurPipeline);
    osg::ref_ptr<RTTCamera> bloomFinalPass = new RTTCamera(m_sourceTexture,buffer0,"resources/shaders/screen.vert", "resources/shaders/bloomFinalPass.frag");
    bloomFinalPass->getDestinationQuadStateSet()->setTextureAttributeAndModes(1,static_cast<RenderTexture*>(buffer1),osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);
    bloomFinalPass->addUniform("_Bloom",1);
    addRenderPass(bloomFinalPass);
}

Bloom::Bloom(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int width,int height,int blurIterations,float blurSpeed,float* luminanceThreshold):
m_sourceTexture(sourceTexture),m_destinationTexture(destinationTexture),m_width(width),m_height(height),m_luminanceThreshold(luminanceThreshold){
    m_blurIterations = blurIterations;
    m_blurSpeed = blurSpeed;
    generateRenderPasses();
}

Bloom::Bloom(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int blurIterations,float blurSpeed,float* luminanceThreshold):
m_sourceTexture(sourceTexture),m_destinationTexture(destinationTexture),m_width(sourceTexture->getTextureWidth()),
m_height(sourceTexture->getTextureHeight()),m_luminanceThreshold(luminanceThreshold){
    m_blurIterations = blurIterations;
    m_blurSpeed = blurSpeed;
    generateRenderPasses();
}

osg::ref_ptr<RenderTexture> Bloom::getDestinationTexture() const{
    return m_destinationTexture;
}

osg::ref_ptr<GaussianBlur> Bloom::getGaussianPipeline() const{
    return m_gaussianBlurPipeline;
}


