#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H
#include "Render/LowRender/RenderPipeline.h"
#include "osg/ref_ptr"
#include <memory>
namespace CSEditor::Render {
class GaussianBlur:public RenderPipelinePostProcess{
public:
    GaussianBlur() = delete;

    GaussianBlur(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int width,int height,int blurIterations,float blurSpeed);

    GaussianBlur(RenderTexture* sourceTexture,RenderTexture* destinationTexture,int blurIterations,float blurSpeed);
    
    osg::ref_ptr<RenderTexture> getDestinationTexture() const;
private:
    void generateRenderPasses();
private:
    int m_width,m_height;
    int m_blurIterations;
    float m_blurSpeed;
    osg::ref_ptr<RenderTexture> m_sourceTexture,m_destinationTexture;

};

}

#endif