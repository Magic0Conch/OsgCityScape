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
#include "Resources/Loaders/ShaderLoader.h"

using namespace CSEditor::ECS;

void Material::serialize(Json &jsonObject){

}

Material::Material(const std::string& vertPath,const std::string& fragPath){
    m_stateSet = new osg::StateSet();
    m_vertPath = vertPath;
    m_fragPath = fragPath;
    m_program = Resources::ShaderLoader::create(vertPath, fragPath);    
    m_stateSet->setAttribute(m_program.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);    
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
        m_attributes[texturePair.first] = texture;
    }

    const auto intsJson = jsonObject["ints"];
    for (const auto &intValue : intsJson.object_items()) {
        m_attributes[intValue.first]=intValue.second.int_value();
    }

    const auto floatsJson = jsonObject["floats"];
    for (const auto &floatValue : floatsJson.object_items()) {
        m_attributes[floatValue.first] = floatValue.second.number_value();
    }

    const auto vec2sJson = jsonObject["vec2s"];
    for (const auto &vec2 : vec2sJson.object_items()) {
        m_attributes[vec2.first] = osg::Vec2f(vec2.second["x"].number_value(), vec2.second["y"].number_value());
    }

    const auto vec3sJson = jsonObject["vec3s"];
    for (const auto &vec3 : vec3sJson.object_items()) {
        m_attributes[vec3.first] = osg::Vec3f(vec3.second["x"].number_value(), vec3.second["y"].number_value(), vec3.second["z"].number_value());
    }

    const auto vec4sJson = jsonObject["vec4s"];
    for (const auto &vec4 : vec4sJson.object_items()) {
        m_attributes[vec4.first] = osg::Vec4f(vec4.second["x"].number_value(), vec4.second["y"].number_value(), vec4.second["z"].number_value(), vec4.second["w"].number_value());
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
    // for (const auto &textureEntry : m_textures) {
    //     m_stateSet->setTextureAttributeAndModes(textureUnit, textureEntry.second.get());
    //     m_program->addBindAttribLocation(textureEntry.first, textureUnit);
    //     textureUnit++;
    // }

    m_stateSet->setAttributeAndModes(m_program.get());

    for (const auto& pair : m_attributes) {
        if (isType<int>(pair.second)) {
            osg::ref_ptr<osg::Uniform> intUniform = new osg::Uniform(pair.first.c_str(), std::any_cast<int>(pair.second));
            m_stateSet->addUniform(intUniform.get());
        } else if (isType<float>(pair.second)) {
            osg::ref_ptr<osg::Uniform> floatUniform = new osg::Uniform(pair.first.c_str(), std::any_cast<float>(pair.second));
            m_stateSet->addUniform(floatUniform.get());
        } else if (isType<osg::ref_ptr<osg::Texture2D>>(pair.second)) {
            osg::ref_ptr<osg::Uniform> textureUniform = new osg::Uniform(pair.first.c_str(), 0); // Assuming the texture unit is 0
            m_stateSet->setTextureAttributeAndModes(textureUnit, std::any_cast<osg::ref_ptr<osg::Texture2D>>(pair.second).get());
            m_program->addBindAttribLocation(pair.first, textureUnit);
            textureUnit++;            
        } else if (isType<osg::Vec2f>(pair.second)) {
            osg::Vec2f vec = std::any_cast<osg::Vec2f>(pair.second);
            osg::ref_ptr<osg::Uniform> vec2Uniform = new osg::Uniform(pair.first.c_str(), vec);
            m_stateSet->addUniform(vec2Uniform.get());
        } else if (isType<osg::Vec3f>(pair.second)) {
            osg::Vec3f vec = std::any_cast<osg::Vec3f>(pair.second);
            osg::ref_ptr<osg::Uniform> vec3Uniform = new osg::Uniform(pair.first.c_str(), vec);
            m_stateSet->addUniform(vec3Uniform.get());
        } else if (isType<osg::Vec4f>(pair.second)) {
            osg::Vec4f vec = std::any_cast<osg::Vec4f>(pair.second);
            osg::ref_ptr<osg::Uniform> vec4Uniform = new osg::Uniform(pair.first.c_str(), vec);
            m_stateSet->addUniform(vec4Uniform.get());
        } else {
            std::cout << "Unknown type" << std::endl;
        }
    }
    onComponentAdded(parentObject);
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

// void Material::setTextures(const std::unordered_map<std::string, osg::ref_ptr<osg::Texture2D>>& textures) { 
//     m_textures = textures; 
// }

// void Material::setInts(const std::unordered_map<std::string, int>& ints) { 
//     m_ints = ints; 
// }

// void Material::setFloats(const std::unordered_map<std::string, float>& floats) { 
//     m_floats = floats; 
// }

void Material::addTexture(const std::string &key,const osg::ref_ptr<osg::Texture2D>value){
    m_attributes[key]=value;
    updateUniform(key,value);
}

void Material::addInt (const std::string &key,int value){
    m_attributes[key]=value;
    updateUniform(key,value);
}

void Material::addFloat(const std::string &key,float value){
    m_attributes[key]=value;
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

void Material::setTexture(const std::string& name, const osg::ref_ptr<osg::Texture2D>& texture) {
    m_attributes[name] = texture;
    auto uniform = m_stateSet->getUniform(name);
    if(uniform){
        uniform->set(texture);
    }
    else {
        spdlog::error(name + "in" + m_vertPath + " or " + m_fragPath + " doesn't exist!");
    }
}

void Material::setInt(const std::string& name, int value) {
    m_attributes[name] = value;
    updateUniform(name,value);
}

void Material::setFloat(const std::string& name, float value) {
    m_attributes[name] = value;
    updateUniform(name,value);
}

void Material::setVec2(const std::string& name, const osg::Vec2f& value) {
    m_attributes[name] = value;
    updateUniform(name,value);
}

void Material::setVec3(const std::string& name, const osg::Vec3f& value) {
    m_attributes[name] = value;
    updateUniform(name,value);
}

void Material::setVec4(const std::string& name, const osg::Vec4f& value) {
    m_attributes[name] = value;
    updateUniform(name,value);
}

void Material::bind(osg::ref_ptr<osg::Geode> geode){
    geode->setStateSet(m_stateSet);
}

void Material::bind(osg::ref_ptr<osg::Geometry> geom){
    geom->setStateSet(m_stateSet);
}

void Material::setMatrix(const std::string& name, const osg::Matrixf& value){
    m_attributes[name] = value;
    updateUniform(name, value);
}

void Material::setMatrix(const std::string& name, const osg::Matrixd& value){
    m_attributes[name] = value;
    osg::ref_ptr<osg::Uniform> uniform = m_stateSet->getUniform(name);
    if(!uniform){
        uniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"_ProjectionMatrix");
        m_stateSet->addUniform(uniform);
    }    
    uniform->set(value);
}

void Material::setBool(const std::string& name, bool value){
    m_attributes[name] = value;
    updateUniform(name, value);
}