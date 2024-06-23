#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Components/Mesh.h"
#include "Resources/ResourceManagement/MaterialManager.h"
#include "osg/Geode"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Core/ECS/Level.h"
#include "Core/ECS/WorldManager.h"
#include "spdlog/spdlog.h"
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <osgUtil/SmoothingVisitor>
using namespace CSEditor::ECS;


void Mesh::serialize(Json &jsonObject){

}

void Mesh::deserialize(Json &jsonObject){
}

void Mesh::commpressTexture(){
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
    m_parentObject = parentObject;   
}

osg::ref_ptr<osg::Geode> Mesh::getMeshNode() const {
    return m_meshNode;
}

void Mesh::setMeshNode(osg::ref_ptr<osg::Geode> node) {
    m_meshNode = node;
    m_meshNode->setNodeMask(0x1);
    auto objectID = m_parentObject.lock()->getID();
    Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->registerNode2IDMap(m_meshNode, objectID);
}

void Mesh::onComponentAdded(std::shared_ptr<Object> parentObject){
    loadResource(parentObject);
    auto objectID = m_parentObject.lock()->getID();
    Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->registerNode2IDMap(m_meshNode, objectID);
}