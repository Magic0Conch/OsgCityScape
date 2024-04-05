#pragma once

#include <osg/Camera>
#include <osg/Texture2D>

namespace CSEditor::Render {

class RenderDepthToTexture : public osg::Camera
{
public:
    RenderDepthToTexture();
    // virtual ~RenderDepthToTexture() {}

    osg::Texture2D* getTexture() const;

private:
    osg::ref_ptr<osg::Texture2D> _texture;
};

}