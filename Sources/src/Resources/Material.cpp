#include "Resources/Material.h"
#include "osg/Geometry"
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
    osg::ref_ptr<osg::Geometry> geom;
}



Material::Material(Material&& rhs){
    setAttributeList(rhs.getAttributeList());
    setShaderProgram(rhs.getShaderProgram());
    setStateset(rhs.getStateSet());
}

Material::Material(const Material& rhs){
    setAttributeList(rhs.getAttributeList());
    setShaderProgram(rhs.getShaderProgram());
    setStateset(rhs.getStateSet());
}

void Material::setAttributeList(const AttributeList& attributeList){
    m_attributeList = attributeList;
}
void Material::setAttributeList(AttributeList&& attributeList){
    m_attributeList = attributeList;
}
void Material::setShaderProgram(osg::ref_ptr<osg::Program> program){
    m_shaderProgram = program;
}

void Material::setStateset(osg::ref_ptr<osg::StateSet> stateset){
    m_stateSet = stateset;
}

osg::ref_ptr<osg::Program> Material::getShaderProgram() const{
    return m_shaderProgram;
}