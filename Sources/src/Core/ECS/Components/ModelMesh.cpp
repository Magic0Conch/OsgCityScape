#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Components/ModelMesh.h"
#include "Resources/Loaders/TextureDownsampleVisitor.h"
#include "Resources/ResourceManagement/MaterialManager.h"
#include "osg/Geode"
#include "osg/Node"
#include "osg/ref_ptr"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Core/ECS/Level.h"
#include "Core/ECS/WorldManager.h"
#include <memory>
#include <string>
#include <osgUtil/SmoothingVisitor>
using namespace CSEditor::ECS;

ModelMesh::ModelMesh(std::string meshPath){
    m_meshPath = meshPath;
}

void ModelMesh::serialize(Json &jsonObject){

}

void ModelMesh::deserialize(Json &jsonObject){
    const auto modelPath = jsonObject["modelPath"].string_value();
    const auto absoluteModelPath = Core::g_runtimeContext.configManager->getAssetFolder()/modelPath;
    m_meshPath = absoluteModelPath.string();
    
    const auto& materialPaths = jsonObject["materials"].array_items();
    for(const auto& materialPath:materialPaths){
        m_materialPaths.emplace_back(materialPath.string_value());
    }    
}

void ModelMesh::loadResourceAsync(std::shared_ptr<Object> parentObject){
    onComponentAdded(parentObject);
}

void ModelMesh::loadResource(std::shared_ptr<Object> parentObject){
    osg::ref_ptr<osg::Node> group = osgDB::readNodeFile(m_meshPath);    
    m_meshNode = group->asGroup()->getChild(0)->asGeode();
    if(m_meshNode){
        TextureDownsampleVisitor visitor;
        group->accept(visitor);
        const auto childrenCount = m_meshNode->getNumChildren();
        for (int i = 0;i<childrenCount;i++) {
            auto geom = m_meshNode->getChild(i)->asGeometry();
            if(geom){
                osgUtil::SmoothingVisitor::smooth(*geom);
            }
        }
    }
    m_parentObject = parentObject;
    m_parentObject.lock()->getTransformComponent().getNode()->addChild(m_meshNode);
    for(const auto& materialPath:m_materialPaths){
        auto matertial = Resources::MaterialManager::getInstance().getMaterial(materialPath);
        matertial->loadResource(parentObject);
    }
    m_meshNode->setNodeMask(0x1);
    commpressTexture();
    // onComponentAdded(parentObject);
}

void ModelMesh::setMeshPath(const std::string& meshPath){
    m_meshPath = meshPath;
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(m_meshPath);
    osg::ref_ptr<osg::Geode> geode = node->asGroup()->getChild(0)->asGeode();
    setMeshNode(geode);
}

const std::string& ModelMesh::getMeshPath(){
    return m_meshPath;
}


void ModelMesh::onComponentAdded(std::shared_ptr<Object> parentObject){
    loadResource(parentObject);
    auto objectID = m_parentObject.lock()->getID();
    Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->registerNode2IDMap(m_meshNode, objectID);
}