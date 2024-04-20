#include "Core/ECS/Components/Material.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include <memory>
#include <string>
#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Components/Mesh.h"
#include "spdlog/spdlog.h"
#include <osgDB/ReadFile>
using namespace CSEditor::ECS;

void Material::serialize(Json &jsonObject){

}

void Material::deserialize(Json &jsonObject){
    const auto name = jsonObject["name"].string_value();
    const auto shaderJson = jsonObject["shader"];
    
    const auto vertPath = Core::g_runtimeContext.configManager->getShaderFolder() / shaderJson["vertPath"].string_value();
    m_vertPath = vertPath.string();
    const auto fragPath = Core::g_runtimeContext.configManager->getShaderFolder() / shaderJson["fragPath"].string_value();
    m_fragPath = fragPath.string();

    const auto texturesJson = jsonObject["textures"];
    for (const auto &texturePair : texturesJson.object_items()) {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
        const auto texturePath = (Core::g_runtimeContext.configManager->getMaterialFolder()/ texturePair.second["path"].string_value()).string();
        auto image = osgDB::readImageFile(texturePath);
        texture->setImage(image);
        m_textures[texturePair.first] = texture;
    }

    const auto intsJson = jsonObject["ints"];
    for (const auto &intValue : intsJson.object_items()) {
        m_ints[intValue.first]=intValue.second.int_value();
    }

    const auto floatsJson = jsonObject["floats"];
    for (const auto &floatValue : floatsJson.object_items()) {
        m_floats[floatValue.first] = floatValue.second.number_value();
    }

    const auto vec2sJson = jsonObject["vec2s"];
    for (const auto &vec2 : vec2sJson.object_items()) {
        m_vec2s[vec2.first] = osg::Vec2f(vec2.second["x"].number_value(), vec2.second["y"].number_value());
    }

    const auto vec3sJson = jsonObject["vec3s"];
    for (const auto &vec3 : vec3sJson.object_items()) {
        m_vec3s[vec3.first] = osg::Vec3f(vec3.second["x"].number_value(), vec3.second["y"].number_value(), vec3.second["z"].number_value());
    }

    const auto vec4sJson = jsonObject["vec4s"];
    for (const auto &vec4 : vec4sJson.object_items()) {
        m_vec4s[vec4.first] = osg::Vec4f(vec4.second["x"].number_value(), vec4.second["y"].number_value(), vec4.second["z"].number_value(), vec4.second["w"].number_value());
    }
    
}

void Material::loadResource(std::shared_ptr<Object> parentObject){
    m_program = new osg::Program;
    m_program->addShader(osgDB::readShaderFile(osg::Shader::VERTEX, m_vertPath));
    m_program->addShader(osgDB::readShaderFile(osg::Shader::FRAGMENT, m_fragPath));
    m_parentObject = parentObject;
    auto meshNodeComponent = parentObject->getComponent<Mesh>();
    auto meshNode = meshNodeComponent->getMeshNode();

    m_stateSet = meshNode->getOrCreateStateSet();

    unsigned int textureUnit = 0; 
    for (const auto &textureEntry : m_textures) {
        m_stateSet->setTextureAttributeAndModes(textureUnit, textureEntry.second.get());
        m_program->addBindAttribLocation(textureEntry.first, textureUnit);
        textureUnit++;
    }

    m_stateSet->setAttributeAndModes(m_program.get());

    // 设置uniform变量
    for (const auto &uniform : m_vec3s) {
        osg::ref_ptr<osg::Uniform> vec3Uniform = new osg::Uniform(uniform.first.c_str(), uniform.second);
        m_stateSet->addUniform(vec3Uniform.get());
    }

    for (const auto &uniform : m_vec4s) {
        osg::ref_ptr<osg::Uniform> vec4Uniform = new osg::Uniform(uniform.first.c_str(), uniform.second);
        m_stateSet->addUniform(vec4Uniform.get());
    }

    for (const auto &uniform : m_floats) {
        osg::ref_ptr<osg::Uniform> floatUniform = new osg::Uniform(uniform.first.c_str(), uniform.second);
        m_stateSet->addUniform(floatUniform.get());
    }

    for (const auto &uniform : m_ints) {
        osg::ref_ptr<osg::Uniform> intUniform = new osg::Uniform(uniform.first.c_str(), uniform.second);
        m_stateSet->addUniform(intUniform.get());
    }    
    onComponentAdded();
}

Material::Material(){}

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

void Material::setTextures(const std::unordered_map<std::string, osg::ref_ptr<osg::Texture2D>>& textures) { 
    m_textures = textures; 
}

void Material::setInts(const std::unordered_map<std::string, int>& ints) { 
    m_ints = ints; 
}

void Material::setFloats(const std::unordered_map<std::string, float>& floats) { 
    m_floats = floats; 
}

void Material::addTexture(const std::string &key,const osg::ref_ptr<osg::Texture2D>value){
    m_textures[key]=value;
    updateUniform(key,value);
}

void Material::addInt (const std::string &key,int value){
    m_ints[key]=value;
    updateUniform(key,value);
}

void Material::addFloat(const std::string &key,float value){
    m_floats[key]=value;
    updateUniform(key,value);
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

const std::unordered_map<std::string,osg::ref_ptr<osg::Texture2D>>& Material::getTextures ()const{
    return m_textures;
}

const std::unordered_map<std::string,int>& Material::getInt ()const{
    return m_ints;
}

const std::unordered_map<std::string,float >& Material::getFloat ()const{
    return m_floats;
}

void Material::setTexture(const std::string& name, const osg::ref_ptr<osg::Texture2D>& texture) {
    m_textures[name] = texture;
    auto uniform = m_stateSet->getUniform(name);
    if(uniform){
        uniform->set(texture);
    }
    else {
        spdlog::error(name + "in" + m_vertPath + " or " + m_fragPath + " doesn't exist!");
    }
}

void Material::setInt(const std::string& name, int value) {
    m_ints[name] = value;
    updateUniform(name,value);
}

void Material::setFloat(const std::string& name, float value) {
    m_floats[name] = value;
    updateUniform(name,value);
}

void Material::setVec2(const std::string& name, const osg::Vec2f& value) {
    m_vec2s[name] = value;
    updateUniform(name,value);
}

void Material::setVec3(const std::string& name, const osg::Vec3f& value) {
    m_vec3s[name] = value;
    updateUniform(name,value);
}

void Material::setVec4(const std::string& name, const osg::Vec4f& value) {
    m_vec4s[name] = value;
    updateUniform(name,value);
}