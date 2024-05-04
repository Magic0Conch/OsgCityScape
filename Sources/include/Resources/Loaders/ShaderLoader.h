#ifndef SHADERUTILS_H
#define SHADERUTILS_H
#include <windows.h>
#include <iostream>
#include <osgDB/ReadFile>
#include <osg/StateSet>
#include <osg/ref_ptr>
#include <osg/Program>

namespace CSEditor::Resources {
class ShaderLoader{
public:    
    static osg::ref_ptr<osg::Program> create(const std::string& vertFilePath,const std::string& fragFilePath);
    static osg::ref_ptr<osg::Program> create(const std::string& vertFilePath);
};
}
#endif