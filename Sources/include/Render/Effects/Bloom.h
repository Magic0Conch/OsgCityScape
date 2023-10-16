#ifndef BLOOM_H
#define BLOOM_H
#include "GaussianBlur.h"
#include "osg/ref_ptr"
#include <memory>
namespace CSEditor::Render {
class Bloom:public RenderPipelinePostProcess{

public:
    Bloom() = delete;

    Bloom(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int width,int height,int blurIterations,float blurSpeed,float* luminanceThreshold);
    
    Bloom(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int blurIterations,float blurSpeed,float* luminanceThreshold);
    
    osg::ref_ptr<RenderTexture> getDestinationTexture() const;

    osg::ref_ptr<GaussianBlur> getGaussianPipeline() const;
private:
    void generateRenderPasses();
private:
    int m_width,m_height;
    int m_blurIterations;
    float m_blurSpeed;
    float* m_luminanceThreshold;
    int m_downSample = 2;
    osg::ref_ptr<GaussianBlur> m_gaussianBlurPipeline;
    osg::ref_ptr<RenderTexture> m_sourceTexture,m_destinationTexture;
};

}

#endif