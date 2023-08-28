#ifndef MOVIE_H
#define MOVIE_H
#include <string>
#include <windows.h>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/TextureRectangle>
#include <osg/Image>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Program>
#include <osg/Referenced>
#include <osg/Texture2D>
#include <osg/ref_ptr>
#include <osgDB/FileUtils>
#include <osgDB/Archive>
#include <osg/ImageSequence>
namespace cs {
class Movie:public osg::Referenced{
public:
    Movie(const std::string& moviePath);
    osg::ref_ptr<osg::Geometry> getScreenQuadGeometry() const;
    osg::ref_ptr<osg::ImageSequence>  getImageSequence() const;
    void updateFrame();
    osg::ref_ptr<osg::Texture2D> getFrame() const;
private:
    const std::string m_vertPath = "resources/shaders/screen.vert";
    const std::string m_fragPath = "resources/shaders/screen.frag";
    std::string m_moviePath;
    osg::ref_ptr<osg::ImageSequence> m_imageSequence;
    osg::ref_ptr<osg::Geometry> m_screenQuad;
    osg::ref_ptr<osg::Texture2D> m_frame;
    osgDB::DirectoryContents m_imageList;
private:
    void setStateSetAttribute();
    mutable int m_currentFrameIndex = 0;
}; 
class MovieCallback:public osg::NodeCallback{
public:
    MovieCallback(Movie* const movie);
    virtual void operator()(osg::Node*,osg::NodeVisitor*);
private:
    osg::ref_ptr<Movie> m_movie;
};
}

#endif