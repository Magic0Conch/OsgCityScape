#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Components/Mesh.h"
#include "Resources/ResourceManagement/MaterialManager.h"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include <memory>
#include <string>

using namespace CSEditor::ECS;

void Mesh::serialize(Json &jsonObject){

}

void Mesh::deserialize(Json &jsonObject){
    const auto modelPath = jsonObject["modelPath"].string_value();
    const auto absoluteModelPath = Core::g_runtimeContext.configManager->getAssetFolder()/modelPath;
    m_meshPath = absoluteModelPath.string();
    m_meshNode = osgDB::readNodeFile(m_meshPath);
    const auto& materialPaths = jsonObject["materials"].array_items();
    for(const auto& materialPath:materialPaths){
        m_materialPaths.emplace_back(materialPath.string_value());
    }    
}

void Mesh::loadResource(std::shared_ptr<Object> parentObject){
    m_parentObject = parentObject;
    for(const auto& materialPath:m_materialPaths){
        auto matertial = Resources::MaterialManager::getInstance().getMaterial(materialPath);
        matertial->loadResource(parentObject);
    }
}

void Mesh::setMeshPath(const std::string& meshPath){
    m_meshPath = meshPath;
    setMeshNode(osgDB::readNodeFile(m_meshPath));
}

const std::string& Mesh::getMeshPath(){
    return m_meshPath;
}

osg::ref_ptr<osg::Node> Mesh::getMeshNode() const {
    return m_meshNode;
}

// Setter for m_meshNode
void Mesh::setMeshNode(osg::ref_ptr<osg::Node> node) {
    m_meshNode = node;
}