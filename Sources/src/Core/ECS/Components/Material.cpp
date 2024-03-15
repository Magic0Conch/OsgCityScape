#include "Core/ECS/Components/Material.h"
#include "Editor/Core/RuntimeContext.h"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include <memory>
#include <string>

using namespace CSEditor::ECS;

void Material::serialize(Json &jsonObject){

}

void Material::deserialize(Json &jsonObject){
    const auto name = jsonObject["name"].string_value();
    const auto shaderJson = jsonObject["shader"];
    
    const auto vertPath = Core::g_runtimeContext.configManager->getShaderRoot() / shaderJson["vertPath"].string_value();
    m_vertPath = vertPath.string();
    const auto fragPath = Core::g_runtimeContext.configManager->getShaderRoot() / shaderJson["fragPath"].string_value();
    m_fragPath = fragPath.string();

    // const auto absoluteModelPath = Core::g_runtimeContext.configManager->getAssetFolder()/modelPath;

    // m_meshPath = absoluteModelPath.string();
    // meshNode = osgDB::readNodeFile(m_meshPath);
}

void Material::loadResource(std::shared_ptr<Object> parentObject){
    m_parentObject = parentObject;
    // auto transformNode = parentObject->getTransformComponent().getNode();
    // transformNode->addChild(meshNode);

}

// void Material::setMeshPath(const std::string& meshPath){
//     m_meshPath = meshPath;
//     meshNode = osgDB::readNodeFile(m_meshPath);
// }

// const std::string& Material::getMeshPath(){
//     return m_meshPath;
// }

void Material::setStateSet(osg::ref_ptr<osg::StateSet> stateSet) {
    m_stateSet = stateSet; 
}
void Material::setName(const std::string& name) { 
    m_name = name; 
}

void Material::setVertPath(const std::string& vertPath) { 
    m_vertPath = vertPath; 
}

void Material::setFragPath(const std::string& fragPath) { 
    m_fragPath = fragPath; 
}


void Material::setTextures(const std::unordered_map<std::string, std::string>& textures) { 
    m_textures = textures; 
}

void Material::setInts(const std::unordered_map<std::string, int>& ints) { 
    m_ints = ints; 
}

void Material::setFloats(const std::unordered_map<std::string, float>& floats) { 
    m_floats = floats; 
}


void Material::addTexture(const std::string &key,const std::string &value){
    m_textures[key]=value;
}

void Material::addInt (const std::string &key,int value){
    m_ints[key]=value;
}

void Material::addFloat(const std::string &key,float value){
    m_floats[key]=value;
}

osg::ref_ptr<osg::StateSet> Material::getStateSet() const{
    return 	m_stateSet;
}

const std::string& Material::getName() const{
    return m_name;
}

const std::string& Material::getVertPath() const{
    return	m_vertPath;
}

const std::string& Material::getFragPath() const{
    return m_fragPath;
}

const std::unordered_map<std::string,std::string>& Material::getTextures ()const{
    return m_textures;
}

const std::unordered_map<std::string,int>& Material::getInt ()const{
    return m_ints;
}

const std::unordered_map<std::string,float >& Material::getFloat ()const{
    return m_floats;
}
