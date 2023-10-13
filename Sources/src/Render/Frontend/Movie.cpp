#include "Render/Frontend/Movie.h"
#include "Resources/Loaders/ShaderUtils.h"
#include <osg/FrameBufferObject>
#include <osg/Image>
#include <osg/ImageSequence>
#include <osg/ImageStream>
#include <osg/Program>
#include <osg/StateAttribute>
#include <osg/TextureRectangle>
#include <osg/Uniform>
#include <osg/ref_ptr>
#include <iostream>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osg/Texture2D>


using namespace CSEditor::Resources;

Movie::Movie(const std::string& moviePath)
:m_moviePath(moviePath),m_frame(new osg::Texture2D){
    m_screenQuad = osg::createTexturedQuadGeometry(osg::Vec3(-1,-1,0), osg::Vec3(2,0,0), osg::Vec3(0,2,0));
    m_screenQuad->setVertexAttribArray(0,m_screenQuad->getVertexArray());
    m_screenQuad->setVertexAttribArray(1,m_screenQuad->getTexCoordArray(0));

    m_frame->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    m_frame->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    m_frame->setResizeNonPowerOfTwoHint(false);
    m_imageList = osgDB::getSortedDirectoryContents(m_moviePath);
    m_imageSequence = new osg::ImageSequence;
    int cnt = 0;
    for(const auto& filename:m_imageList){
        std::cerr<<"\rImages remaining: "<<m_imageList.size() - 1 - cnt++<<' '<<std::flush;
        if(filename.size()>3){
            const auto filePath = osgDB::concatPaths(moviePath, filename);
            const auto image = osgDB::readRefImageFile(filePath);
            m_imageSequence->addImage(image);
        }
    }
    setStateSetAttribute();        
}

void Movie::setStateSetAttribute(){
    auto stateSet = m_screenQuad->getOrCreateStateSet();
    stateSet->setTextureAttributeAndModes(0,m_frame);
    stateSet->addUniform(new osg::Uniform("_MainTex",0));
    ShaderUtils::setShaderProgram(stateSet, m_vertPath, m_fragPath);
}

osg::ref_ptr<osg::Geometry> Movie::getScreenQuadGeometry() const{
    return m_screenQuad;
}

osg::ref_ptr<osg::ImageSequence> Movie::getImageSequence() const{
    return m_imageSequence;
}

void Movie::updateFrame(){
    auto stateSet = m_screenQuad->getOrCreateStateSet();
    auto index = (m_currentFrameIndex++)%(m_imageList.size()-2);
    m_frame->setImage(m_imageSequence->getImage(index));
    stateSet->setTextureAttributeAndModes(0,m_frame);
}

osg::ref_ptr<osg::Texture2D> Movie::getFrame() const{
    return m_frame;
}

MovieCallback::MovieCallback(Movie* const movie):m_movie(movie){}

void MovieCallback::operator()(osg::Node* node,osg::NodeVisitor* nv){    
    m_movie->updateFrame();
}