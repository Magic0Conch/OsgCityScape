#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H
#include "Render/RenderPipelinePostProcess.h"
#include "Render/RenderTexture.h"
#include "osg/ref_ptr"
#include <memory>
namespace cs {
class GaussianBlur:public RenderPipelinePostProcess{
private:
    int m_width,m_height;
    int m_blurIterations;
    float m_blurSpeed;
    osg::ref_ptr<RenderTexture> m_sourceTexture,m_destinationTexture;
    void generateRenderPasses(){
        osg::ref_ptr<RenderTexture> buffer0 = new RenderTexture(m_width,m_height);
        osg::ref_ptr<RenderTexture> buffer1 = new RenderTexture(m_width,m_height);
        addRenderPass(new RTTCamera(m_sourceTexture,buffer0,"resources/shaders/screen.vert", "resources/shaders/screen.frag"));
        const auto texelSize = new osg::Uniform("_MainTex_TexelSize",osg::Vec2f(1./m_width,1./m_height));
        for (int i = 0; i<m_blurIterations; i++) {
            //vertical pass
            osg::ref_ptr<RTTCamera> verticalPass = new RTTCamera(buffer0,buffer1,
            "resources/shaders/gaussianBlurVertical.vert", "resources/shaders/gaussianBlur.frag");
            verticalPass->addUniform("_BlurSize",1.f+i*m_blurSpeed);
            verticalPass->addUniform(texelSize);
            addRenderPass(verticalPass);
            //horizontal pass
            osg::ref_ptr<RTTCamera> horizontalPass = new RTTCamera(buffer1,buffer0,
            "resources/shaders/gaussianBlurHorizontal.vert", "resources/shaders/gaussianBlur.frag");
            horizontalPass->addUniform("_BlurSize",1.f+i*m_blurSpeed);
            horizontalPass->addUniform(texelSize);
            addRenderPass(horizontalPass);
        }
        addRenderPass(new RTTCamera(buffer0,m_destinationTexture,"resources/shaders/screen.vert", "resources/shaders/screen.frag"));
    }
public:
    GaussianBlur() = delete;

    GaussianBlur(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int width,int height,int blurIterations,float blurSpeed):
    m_sourceTexture(sourceTexture),m_destinationTexture(destinationTexture),m_width(width),m_height(height),m_blurIterations(){
        // m_blurIterations = std::make_shared<int>();
        // m_blurSpeed = std::make_shared<float>();
        // m_blurIterations.reset(blurIterations);
        // m_blurSpeed.reset(blurSpeed);
        m_blurIterations = blurIterations;
        m_blurSpeed = blurSpeed;
        generateRenderPasses();
    }

    GaussianBlur(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int blurIterations,float blurSpeed):
    m_sourceTexture(sourceTexture),m_destinationTexture(destinationTexture),m_width(sourceTexture->getTextureWidth()),m_height(sourceTexture->getTextureHeight()){
        // m_blurIterations = std::make_shared<int>();
        // m_blurSpeed = std::make_shared<float>();
        // m_blurIterations.reset(blurIterations);
        // m_blurSpeed.reset(blurSpeed);
        m_blurIterations = blurIterations;
        m_blurSpeed = blurSpeed;
        generateRenderPasses();
    }
    
    osg::ref_ptr<RenderTexture> getDestinationTexture() const{
        return m_destinationTexture;
    }

};

}

#endif