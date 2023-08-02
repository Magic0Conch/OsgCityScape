#ifndef SHADERUTILS_H
#define SHADERUTILS_H
#include <windows.h>
#include <iostream>
#include <osgDB/ReadFile>
#include <osg/StateSet>
#include <osg/ref_ptr>
#include <osg/Program>

namespace cs {
class ShaderUtils{
public:
    static void setShaderProgram(osg::ref_ptr<osg::StateSet> stateSet,const std::string& vert,const std::string& frag);
};
}
#endif