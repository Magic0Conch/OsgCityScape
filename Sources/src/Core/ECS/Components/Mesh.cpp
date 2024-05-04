#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Components/Mesh.h"
#include "Resources/ResourceManagement/MaterialManager.h"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Core/ECS/Level.h"
#include "Core/ECS/WorldManager.h"
#include <memory>
#include <string>
#include <thread>
#include <osgUtil/SmoothingVisitor>
using namespace CSEditor::ECS;

void Mesh::serialize(Json &jsonObject){

}

void Mesh::deserialize(Json &jsonObject){
    const auto modelPath = jsonObject["modelPath"].string_value();
    const auto absoluteModelPath = Core::g_runtimeContext.configManager->getAssetFolder()/modelPath;
    m_meshPath = absoluteModelPath.string();
    
    const auto& materialPaths = jsonObject["materials"].array_items();
    for(const auto& materialPath:materialPaths){
        m_materialPaths.emplace_back(materialPath.string_value());
    }    
}

void Mesh::loadResourceAsync(std::shared_ptr<Object> parentObject){
    m_meshNode = osgDB::readNodeFile(m_meshPath);
    m_parentObject = parentObject;
    m_parentObject.lock()->getTransformComponent().getNode()->addChild(m_meshNode);
    for(const auto& materialPath:m_materialPaths){
        auto matertial = Resources::MaterialManager::getInstance().getMaterial(materialPath);
        matertial->loadResource(parentObject);
    }
    m_meshNode->setNodeMask(0x1);
    commpressTexture();
}

void Mesh::commpressTexture(){
    // return;
    auto geodeGroup = m_meshNode->asGroup()->getChild(0)->asGeode();
    if(geodeGroup){
        const auto childrenCount = geodeGroup->getNumChildren();
        for (int i = 0;i<childrenCount;i++) {
            auto geom = geodeGroup->getChild(i)->asGeometry();
            auto stateset = geom->getOrCreateStateSet();
            osg::StateSet::TextureAttributeList& texAttribList = stateset->getTextureAttributeList();
            for (int unit = 0; unit < texAttribList.size(); unit++) {
                osg::Texture2D* tex2D = dynamic_cast<osg::Texture2D*>(stateset->getTextureAttribute(i, osg::StateAttribute::TEXTURE));
                if (tex2D) {
                    tex2D->setInternalFormat(GL_COMPRESSED_RGB_S3TC_DXT1_EXT);
                }
            }
            if(geom){
                osgUtil::SmoothingVisitor::smooth(*geom);
            }
        }
    }
}

void Mesh::loadResource(std::shared_ptr<Object> parentObject){
    m_meshNode = osgDB::readNodeFile(m_meshPath);
    auto geodeGroup = m_meshNode->asGroup()->getChild(0)->asGeode();
    if(geodeGroup){
        const auto childrenCount = geodeGroup->getNumChildren();
        for (int i = 0;i<childrenCount;i++) {
            auto geom = geodeGroup->getChild(i)->asGeometry();
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
    onComponentAdded();
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
    m_meshNode->setNodeMask(0x1);
    commpressTexture();
    // Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->nodeToObjectID[m_meshNode] = m_parentObject.lock()->getID();
}

void Mesh::onComponentAdded(){
    // auto objectID = m_parentObject.lock()->getID();
    // Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->nodeToObjectID[m_meshNode] = objectID;
}