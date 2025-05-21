#include "Resources/Loaders/ShaderLoader.h"
#include "osg/Shader"

using namespace CSEditor::Resources;

osg::ref_ptr<osg::Program> ShaderLoader::create(const std::string& vertFilePath,const std::string& fragFilePath){
    auto vertShader = osgDB::readRefShaderFile(osg::Shader::VERTEX,vertFilePath);
    auto fragShader = osgDB::readRefShaderFile(osg::Shader::FRAGMENT,fragFilePath);
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(vertShader);
    program->addShader(fragShader);
    return program;
}

osg::ref_ptr<osg::Program> ShaderLoader::create(const std::string& vertFilePath){
    auto vertShader = osgDB::readRefShaderFile(osg::Shader::VERTEX,vertFilePath);
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(vertShader);
    return program;
}

