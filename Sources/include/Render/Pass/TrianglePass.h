#pragma once

#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Matrixd>
#include <osg/ref_ptr>

namespace CSEditor::Render {

class TrianglePass : public osg::Camera
{
public:
    TrianglePass();
    void setup(
        osg::ref_ptr<osg::GraphicsContext> gc,
        int width,
        int height,
        osg::ref_ptr<osg::Texture2D> inputTexture,
        unsigned int cullMask = 0x1,
        int renderOrder = 0
    );
    virtual ~TrianglePass();

    void setProjectionMatrix(const osg::Matrixd& proj);

private:
    void createTriangleShader(osg::StateSet* ss);
    osg::ref_ptr<osg::Texture2D> _inputTexture;
    osg::ref_ptr<osg::Uniform> _projectionUniform;
};

}
