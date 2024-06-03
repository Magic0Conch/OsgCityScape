#pragma once
#include "Core/ECS/Components/Mesh.h"
#include "Core/ECS/Level.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Core/Math/MatrixHelper.h"
#include "Editor/Core/RuntimeContext.h"
#include "GL/glcorearb.h"
#include "Render/Effects/OutlineFX.h"
#include "Render/LowRender/DisplayTexture.h"
#include "Render/LowRender/RenderColorToTexture.h"
#include "Render/LowRender/RenderDepthToTexture.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Resources/ResourceType/Common/Level.h"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "osg/Callback"
#include "osg/Camera"
#include "osg/FrameBufferObject"
#include "osg/Group"
#include "osg/Math"
#include "osg/Matrix"
#include "osg/Matrixd"
#include "osg/NodeVisitor"
#include "osg/StateAttribute"
#include "osg/Texture2D"
#include "osg/Texture2DArray"
#include "osg/Vec3f"
#include "osg/ref_ptr"
#include "osgDB/ReadFile"
#include "Windowing/Window.h"
#include "Core/ECS/WorldManager.h"
#include <memory>
#include <iostream>
#include <osgDB/WriteFile>
#include <vector>
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Core/Math/MatrixHelper.h"
#include <osg/CullFace>
#include "Core/Math/Math.h"
#include <osgFX/Outline>
#include "Render/Entities/ArrowGizmo.h"

namespace CSEditor::Render {

class GeometryTextureSetterVisitor : public osg::NodeVisitor
{
public:
    GeometryTextureSetterVisitor(const std::string& slaveCameraName)
        : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
          _slaveCameraName(slaveCameraName)
    {
    }

    virtual void apply(osg::Camera& camera) override
    {
        if (camera.getName() == _slaveCameraName)
        {
            for (unsigned int i = 0; i < camera.getNumChildren(); ++i)
            {
                camera.getChild(i)->accept(*this);
            }
        }
        else
        {
            traverse(camera);
        }
    }

    virtual void apply(osg::Geometry& geometry) override
    {

        osg::StateSet* stateSet = geometry.getOrCreateStateSet();
        if (stateSet)
        {
            auto node = osgDB::readNodeFile("1.obj");
            
            // 创建一个纹理对象，并设置到StateSet中
            osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
            // 这里需要设置纹理的图像数据，例如：
            // texture->setImage(osgDB::readImageFile("texture.png"));

            // 将纹理设置到StateSet的纹理单元0
            stateSet->setTextureAttributeAndModes(0, texture.get());
        }

        traverse(geometry);
    }

private:
    std::string _slaveCameraName;
};




class RenderSystem{
public:
    RenderSystem(){
        m_level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
        m_levelResource = m_level->getLevelResource();
        m_mainCamera = Core::g_runtimeContext.windowSystem->getMainCamera();
        m_depthPass = new Render::RenderDepthToTexture;
        m_opaqueEffectPass = new osg::Camera;
        m_textureProjectionPass = std::make_unique<Render::TextureProjectionPass>(m_mainCamera);
        initialize();
    };

    ~RenderSystem(){
    };

    void createLightMatrices(){
        using namespace CSEditor::Math;
        float fovy = CSEditor::Math::Math::focal2fovEuler(1.55);
        auto viewMatrix = MatrixHelper::glmToOsgMatrix(glm::dmat4(0.671205,-0.474751,0.569293,0,0.740948,0.452385,-0.496331,0,-0.021905,0.754957,0.655409,0,-39.120093,-26.573260,-168.144351,1));
        m_mainCamera->setProjectionMatrixAsPerspective(fovy, 1.33333, 5, 1500);
        const auto projectionMatrix = m_mainCamera->getProjectionMatrix();
        m_mainProjectionMatrix = projectionMatrix;
        m_mainCamera->setViewMatrix(viewMatrix);
        auto viewProjectionMatrix = viewMatrix * m_mainProjectionMatrix;
        osg::Matrixd rotationMatrix(1, 0, 0, 0,
                                    0, 0, -1, 0,
                                    0, 1, 0, 0,
                                    0, 0, 0, 1);
        auto lightMatrix = rotationMatrix * viewMatrix * m_mainProjectionMatrix;
        m_lightMatrices.emplace_back(lightMatrix);

        m_depthPass->setViewMatrix(lightMatrix);
        m_depthPass->setProjectionMatrix(projectionMatrix);

        m_opaqueEffectPass->setProjectionMatrix(projectionMatrix);
        m_textureProjectionPass->setProjectionMatrix(projectionMatrix);
    }

    void createResources(){
        m_depthArray = new osg::Texture2DArray;
        m_depthArray->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        m_depthArray->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        m_depthArray->setInternalFormat(GL_DEPTH_COMPONENT);
        m_depthArray->setSourceFormat(GL_DEPTH_COMPONENT);
        m_depthArray->setSourceType(GL_FLOAT);
        m_depthArray->setTextureSize(width, height, 16);

        //create projection color texture
        osg::Texture2D* projectionTexture = new osg::Texture2D;
        const auto texturePath = (Core::g_runtimeContext.configManager->getMaterialFolder()/ "DJI_0314.JPG").string();
        auto projectionImage = osgDB::readImageFile(texturePath);
        projectionImage->scaleImage(width, height, 1);
        projectionTexture->setImage(projectionImage);
        m_colorTextureVector.emplace_back(projectionTexture);        
    }

    void setupRenderPasses(){
        //setup 
        auto viewer = Core::g_runtimeContext.viewer;
        auto graphicsContext = Core::g_runtimeContext.windowSystem->getGraphicsContext();
        auto mainViewport = Core::g_runtimeContext.windowSystem->getViewport();

        //depth pass        
        m_depthPass->setGraphicsContext(graphicsContext);
        m_depthPass->setViewport(0,0,width,height);
        m_depthPass->attach(osg::Camera::DEPTH_BUFFER,m_depthArray.get(),0,0);        
        m_depthPass->setCullMask(0x1);
        m_depthPass->setRenderOrder(osg::Camera::PRE_RENDER, 0);
        viewer->addSlave(m_depthPass);

        //texture projection pass

        m_textureProjectionPass->setTextureArray(m_depthArray, m_colorTextureVector, m_lightMatrices);
        m_mainCamera->setCullMask(0x1);
        m_mainCamera->setRenderOrder(osg::Camera::PRE_RENDER, 1);
        m_mainColorTexture = m_textureProjectionPass->getColorTexture();
        m_mainDepthStencilTexture = m_textureProjectionPass->getDepthStencilTexture();
        
        //opaque effect pass
        m_opaqueEffectPass->setGraphicsContext(graphicsContext);
        m_opaqueEffectPass->setViewport(mainViewport);
        m_opaqueEffectPass->setCullMask(0x2);
        m_opaqueEffectPass->setCullingMode(m_mainCamera->getCullingMode() & ~osg::CullSettings::SMALL_FEATURE_CULLING);
        m_opaqueEffectPass->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        m_opaqueEffectPass->attach(osg::Camera::COLOR_BUFFER0, m_mainColorTexture);
        m_opaqueEffectPass->attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER, m_mainDepthStencilTexture);
        m_opaqueEffectPass->setClearMask(GL_NONE);
        m_opaqueEffectPass->setRenderOrder(osg::Camera::PRE_RENDER,2);
        viewer->addSlave(m_opaqueEffectPass);
    }

    void initialize(){
        createLightMatrices();
        createResources();
        setupRenderPasses();
    };

    void tick(float deltaTime){
        if(m_level->isSelectedObjectDirty()){
            auto selectedObject = m_level->getSelectedObject();
            auto transform = selectedObject->getTransformComponent();
            auto mesh = selectedObject->getComponent<ECS::Mesh>();
            auto meshGroupNode = mesh->getMeshNode()->asGroup();
            auto geodeNode = meshGroupNode->getChild(0);
            
            if(m_level->hasLastSelectedObject()){
                auto lastSeletedObject = m_level->getLastSelectedObject();
                auto lastMesh = lastSeletedObject->getComponent<ECS::Mesh>();
                auto lastMeshGroupNode = lastMesh->getMeshNode()->asGroup();
                // auto outlineNode = lastMeshGroupNode->getChild(0);
                // auto lastGeodeNode = dynamic_cast<osgFX::OutlineFX*>(outlineNode)->getChild(0);
                lastMeshGroupNode->removeChild(1);
                lastMeshGroupNode->removeChildren(1,4);
                // lastMeshGroupNode->replaceChild(outlineNode, lastGeodeNode);
                lastMeshGroupNode->setNodeMask(0x1);
            }
            m_lastSelectedObjectID = m_level->getSelectedObjectID();
            m_level->setLastSelectedObjectID(m_lastSelectedObjectID);
            

            osg::ref_ptr<osgFX::OutlineFX> pOutLine = new osgFX::OutlineFX;
            pOutLine->setProjectionMatrix(m_mainProjectionMatrix);
            pOutLine->setWidth(1);
            pOutLine->setColor(osg::Vec4(1,1,0,1));
            pOutLine->addChild(geodeNode);
            // meshGroupNode->replaceChild(geodeNode, pOutLine);
            meshGroupNode->addChild(pOutLine);
            meshGroupNode->setNodeMask(0x2);
            auto bound = meshGroupNode->computeBound();
            auto center = bound.center();
            osg::Vec3f arrowStart = osg::Vec3f(center.x(), center.y(), center.z());
            float arrowSize = 5;
            ArrowGizmo* arrowX = new ArrowGizmo(arrowStart, arrowStart + osg::Vec3(arrowSize,0,0), osg::Vec4(1,0,0,1));
            ArrowGizmo* arrowY = new ArrowGizmo(arrowStart, arrowStart + osg::Vec3(0,arrowSize,0), osg::Vec4(0,1,0,1));
            ArrowGizmo* arrowZ = new ArrowGizmo(arrowStart, arrowStart + osg::Vec3(0,0,arrowSize), osg::Vec4(0,0,1,1));
            meshGroupNode->addChild(arrowX);            
            meshGroupNode->addChild(arrowY);            
            meshGroupNode->addChild(arrowZ);            
            m_level->setSelectedObjectDirty(false);
        }
    };
    
private:
    const int width = 1200;
    const int height = 900;
    std::shared_ptr<ResourceType::Level> m_levelResource;
    std::shared_ptr<ECS::Level> m_level;
    osg::ref_ptr<osg::Texture2DArray> m_depthArray;
    std::vector<osg::Matrixd> m_lightMatrices;
    osg::Matrixd m_mainProjectionMatrix;
    osg::ref_ptr<Render::RenderDepthToTexture> m_depthPass;
    std::unique_ptr<Render::TextureProjectionPass> m_textureProjectionPass;
    ECS::ObjectID m_lastSelectedObjectID = -1;
    std::vector<osg::Texture2D *> m_colorTextureVector;
    osg::ref_ptr<osg::Camera> m_mainCamera;
    osg::ref_ptr<osg::Camera> m_opaqueEffectPass;
    osg::ref_ptr<osg::Texture2D> m_mainColorTexture;
    osg::ref_ptr<osg::Texture2D> m_mainDepthStencilTexture;

};
}