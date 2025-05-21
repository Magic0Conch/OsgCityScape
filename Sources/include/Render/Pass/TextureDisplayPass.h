#pragma once

#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Uniform>

namespace CSEditor::Render {

class TextureDisplayPass : public osg::Camera
{
public:
    TextureDisplayPass();
    void setup(osg::ref_ptr<osg::GraphicsContext> gc,
               int width, int height,
               osg::ref_ptr<osg::Texture2D> inputTexture,
               int renderOrder = 999);
    void setShader(const std::string& vertPath, const std::string& fragPath);

private:
    osg::ref_ptr<osg::Uniform> _mainTextureUniform;
    osg::ref_ptr<osg::Geode> _fullscreenQuad;
};

}
