#include "Resources/Material.h"
#include "osg/StateSet"
using namespace CSEditor::Resources;

Material::Material(const std::string& vertPath,const std::string& fragPath){
    m_stateSet = new osg::StateSet();
    m_shaderProgram = ShaderLoader::create(vertPath, fragPath);    
    m_stateSet->setAttribute(m_shaderProgram.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);    
}

void Material::addUniform(osg::Uniform* uniform){
    m_stateSet->addUniform(uniform);
}

Material::Material(const osg::ref_ptr<osg::Program> shaderProgram)
:m_shaderProgram(shaderProgram){
}

std::vector<std::pair<std::shared_ptr<std::string>, std::shared_ptr<Material::Attribute>>> Material::getAttributeList() const{
    return m_attributeList;
}

osg::ref_ptr<osg::StateSet> Material::getStateSet() const{
    return m_stateSet;
}

void Material::bind(osg::ref_ptr<osg::Geode> geode){
    geode->setStateSet(m_stateSet);
}