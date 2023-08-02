#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H
#include <Windows.h>
#include "osg/CopyOp"
#include "osg/Referenced"
#include "osg/Texture2D"
#include "osg/ref_ptr"

namespace cs{
class RenderTexture:public osg::Texture2D{
public:
    RenderTexture(int width,int height){
        setInternalFormat(GL_RGBA);
        setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
        setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
        setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
        setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
        setTextureSize(width, height);       
        setResizeNonPowerOfTwoHint(false);

    }
private:

};
}
#endif