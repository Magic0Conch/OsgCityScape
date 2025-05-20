#pragma once
#include "Render/Pass/ObjectPickerPass.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"


namespace CSEditor::Render {
    void printChildInfo(osg::Group* group) {
        if (!group) return;

        unsigned int numChildren = group->getNumChildren();
        std::cout << "Number of children: " << numChildren << std::endl;

        for (unsigned int i = 0; i < numChildren; ++i) {
            osg::Node* child = group->getChild(i);
            std::cout << "Child " << i << ": " << child->getName() << " (Type: " << child->className() << ")" << std::endl;
        }
    }

    ObjectPickerPass::ObjectPickerPass(std::shared_ptr<osg::Matrixd> projectionMatrix){
        m_level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel(); 
        m_projectionMatrix = projectionMatrix;
        m_outlinePass = new osgFX::OutlineFX;
    }

    void ObjectPickerPass::highlightSelectedObject(const RenderPipelineState renderPipelineState){
        if(m_level->isSelectedObjectDirty()){
            auto selectedObject = m_level->getSelectedObject();
            auto mesh = selectedObject->getComponent<ECS::ModelMesh>();
            if(m_level->hasLastSelectedObject()){
                auto lastSeletedObject = m_level->getLastSelectedObject();
                auto lastMesh = lastSeletedObject->getComponent<ECS::ModelMesh>();
                auto lastMeshGroupNode = lastMesh->getMeshNode()->asGroup();
                printChildInfo(lastMeshGroupNode);
                lastMeshGroupNode->removeChild(1);
                lastMeshGroupNode->removeChildren(1,4);
                lastMeshGroupNode->setNodeMask(0x1);
                printChildInfo(lastMeshGroupNode);
            }

            if(mesh == nullptr){
                m_lastSelectedObjectID = 0;
                m_level->setLastSelectedObjectID(m_lastSelectedObjectID);
                m_selectedGroupNode = nullptr;
                return;
            }
            auto transform = selectedObject->getTransformComponent();
            m_selectedGroupNode = mesh->getMeshNode()->asGroup();
            auto geodeNode = m_selectedGroupNode->getChild(0);

            m_lastSelectedObjectID = m_level->getSelectedObjectID();
            m_level->setLastSelectedObjectID(m_lastSelectedObjectID);

            
            m_outlinePass->setProjectionMatrix(*m_projectionMatrix);
            m_outlinePass->setWidth(1);
            m_outlinePass->setColor(osg::Vec4(1,1,0,1));
            auto numChildren = m_outlinePass->getNumChildren();
            if (numChildren > 0) {
                m_outlinePass->removeChildren(0, numChildren);
            }
            m_outlinePass->addChild(geodeNode);
            m_selectedGroupNode->addChild(m_outlinePass);
            printChildInfo(m_selectedGroupNode);
            
            auto bound = m_selectedGroupNode->computeBound();
            auto center = bound.center();
            osg::Vec3f arrowStart = osg::Vec3f(0, 0, 0);
            float arrowSize = 5;
            ArrowGizmo* arrowX = new ArrowGizmo(arrowStart, arrowStart + osg::Vec3(arrowSize,0,0), osg::Vec4(1,0,0,1));
            ArrowGizmo* arrowY = new ArrowGizmo(arrowStart, arrowStart + osg::Vec3(0,arrowSize,0), osg::Vec4(0,1,0,1));
            ArrowGizmo* arrowZ = new ArrowGizmo(arrowStart, arrowStart + osg::Vec3(0,0,arrowSize), osg::Vec4(0,0,1,1));
            m_selectedGroupNode->addChild(arrowX);            
            m_selectedGroupNode->addChild(arrowY);            
            m_selectedGroupNode->addChild(arrowZ);            
            m_level->setSelectedObjectDirty(false);
        }

        if(renderPipelineState == RenderPipelineState::Default){
            m_outlinePass->setWidth(1);
            if(m_selectedGroupNode){
                // m_selectedGroupNode->setNodeMask(0x1);
            }
        }
        else if(renderPipelineState == RenderPipelineState::TextureBaking){
            m_outlinePass->setWidth(0);
            if(m_selectedGroupNode){
                m_selectedGroupNode->setNodeMask(0x4);
            }

        }
        
    }

}