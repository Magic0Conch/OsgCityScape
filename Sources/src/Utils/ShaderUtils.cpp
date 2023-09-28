#include "Resources/Loaders/ShaderUtils.h"

using namespace CSEditor::Resources;

void ShaderUtils::setShaderProgram(osg::ref_ptr<osg::StateSet> stateSet, const std::string &vert, const std::string &frag){
    auto vertShader = osgDB::readRefShaderFile(osg::Shader::VERTEX,vert);
    auto fragShader = osgDB::readRefShaderFile(osg::Shader::FRAGMENT,frag);
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(vertShader);
    program->addShader(fragShader);
    stateSet->setAttribute(program.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}