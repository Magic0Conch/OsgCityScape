#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Components/ProceduralMesh.h"
#include "Render/Entities/BaseGeometry.h"
#include "Render/Entities/Circle.h"
#include "Resources/ResourceManagement/MaterialManager.h"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/Node"
#include "osg/ref_ptr"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Core/ECS/Level.h"
#include "Core/ECS/WorldManager.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <thread>
#include <osgUtil/SmoothingVisitor>
using namespace CSEditor::ECS;


void ProceduralMesh::serialize(Json &jsonObject){

}

void ProceduralMesh::deserialize(Json &jsonObject){
}

void ProceduralMesh::loadResource(std::shared_ptr<Object> parentObject){
    m_meshNode = new osg::Geode();
    m_parentObject = parentObject;
    m_parentObject.lock()->getTransformComponent().getNode()->addChild(m_meshNode);
    m_meshNode->setNodeMask(0x1);
    // for(const auto& materialPath:m_materialPaths){
    //     auto matertial = Resources::MaterialManager::getInstance().getMaterial(materialPath);
    //     matertial->loadResource(parentObject);
    // }
    // commpressTexture();
}

void ProceduralMesh::onComponentAdded(std::shared_ptr<Object> parentObject){
    loadResource(parentObject);
    auto objectID = m_parentObject.lock()->getID();
    Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->registerNode2IDMap(m_meshNode, objectID);
}

void ProceduralMesh::addGeometry(osg::ref_ptr<Render::BaseGeometry> geometry, std::shared_ptr<Material> material){
    m_geometries.push_back(geometry);
    m_materials.push_back(material);
    m_meshNode->addChild(geometry);
    material->bind(static_cast<osg::ref_ptr<osg::Geometry>>(geometry));
    
}
std::vector<osg::ref_ptr<CSEditor::Render::BaseGeometry>>& ProceduralMesh::getGeometries(){
    return m_geometries;
}
std::vector<std::shared_ptr<Material>>& ProceduralMesh::getMaterials(){
    return m_materials;
}