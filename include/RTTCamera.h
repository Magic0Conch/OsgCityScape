#ifndef RTTCAMERA_H
#define RTTCAMERA_H
#include <string>
#include <windows.h>
#include "osg/Camera"
#include "osg/Geode"
#include "osg/Group"
#include "osg/Referenced"
#include "osg/Uniform"
#include "osg/Vec4f"
#include "osg/ref_ptr"
#include "RenderTexture.h"
namespace cs{
class RTTCamera:public osg::Camera{
public:
    RTTCamera(RenderTexture* source,RenderTexture* destination,const std::string& vertPath,const std::string& fragPath);
    RTTCamera(osg::Group* source,RenderTexture* destination,const std::string& vertPath,const std::string& fragPath);
    osg::ref_ptr<RenderTexture> getDestinationTexture() const;
    osg::ref_ptr<osg::Geode> getDestinationQuadGeode() const;
    void addUniform(osg::Uniform*);
private:
    osg::ref_ptr<osg::Group> m_scene = nullptr;
    osg::ref_ptr<RenderTexture> m_sourceTexture = nullptr;
    osg::ref_ptr<RenderTexture> m_destinationTexture = nullptr;
    osg::ref_ptr<osg::Geode> m_quadGeode = nullptr;
    
    
    std::string m_vertShaderPath;
    std::string m_fragShaderPath;

    void initialize();
    void setDefaultUniformParameters();
};
}

#endif