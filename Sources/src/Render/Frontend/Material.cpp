#include "Render/Frontend/Material.h"
using namespace CSEditor::Resources;
Material::Material(osg::Geode* source):m_sourceGeode(source){}

Material::Material(osg::Geode* source,const std::string& vertPath,const std::string& fragPath)
:m_sourceGeode(source),m_vertShaderPath(vertPath),m_fragShaderPath(fragPath){
    ShaderUtils::setShaderProgram(m_sourceGeode->getOrCreateStateSet(),m_vertShaderPath, m_fragShaderPath);
}

Material::Material(const std::string& vertPath,const std::string& fragPath)
:m_vertShaderPath(vertPath),m_fragShaderPath(fragPath){}
void Material::addUniform(osg::Uniform* uniform){
    m_sourceGeode->getOrCreateStateSet()->addUniform(uniform);
}

std::vector<std::pair<std::shared_ptr<std::string>, std::shared_ptr<Material::Attribute>>> Material::getAttributeList(){
    return m_attributeList;
}

