#pragma once
#include "Core/ECS/Level.h"
#include "Editor/Core/RuntimeContext.h"
#include "Render/LowRender/DisplayTexture.h"
#include "Render/LowRender/RenderColorToTexture.h"
#include "Render/LowRender/RenderDepthToTexture.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Resources/ResourceType/Common/Level.h"
#include "osg/Camera"
#include "osg/Group"
#include "osg/Matrix"
#include "osg/Texture2D"
#include "osg/ref_ptr"
#include "osgDB/ReadFile"
#include "osgGA/TrackballManipulator"
#include "Windowing/Window.h"
#include "Core/ECS/WorldManager.h"
#include <memory>
#include <locale>
#include <iostream>
#include <osgDB/WriteFile>
#include <vector>
#include "Resources/ResourceManagement/ConfigManager.h"
namespace CSEditor::Render {
class RenderSystem{
public:
    RenderSystem(){
        m_level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
        m_levelResource = m_level->getLevelResource();
        std::cout.ios_base::imbue(std::locale::classic());
        std::ios::sync_with_stdio();
        initialize();
    };

    ~RenderSystem(){

    };

    void initialize(){
        //setup 
        auto viewer = Core::g_runtimeContext.viewer;
        auto graphicsContext = Core::g_runtimeContext.windowSystem->getGraphicsContext();

        const int width = 2048;
        const int height = 2048;

        // initialize the level resource
        viewer->setSceneData(m_level->getRootObject()->getTransformComponent().getNode().get());
        auto objects = m_levelResource->getObjects();
        for (const ResourceType::ObjectInstance& objectInstanceRes:objects) {
            auto curObjectId = m_level->loadObjectInstance(objectInstanceRes);
            auto thisTransform = m_level->getSceneObjectById(curObjectId)->getTransformComponent();
            m_level->getRootObject()->addChild(thisTransform);
        }
        m_level->setIsLoaded(true);

        //render pass
        Render::RenderDepthToTexture *depthPass = new Render::RenderDepthToTexture;
        depthPass->setGraphicsContext(graphicsContext);
        depthPass->setViewport(0,0,width,height);
        depthPass->setProjectionMatrixAsPerspective(60.0f, 1.0, 1.0f, 1000.0f);
        viewer->addSlave(depthPass);

        //depth
        std::vector<osg::Texture2D *> depthTextureVector;
        depthTextureVector.emplace_back(depthPass->getTexture());

        //color
        std::vector<osg::Texture2D *> colorTextureVector;
        osg::ref_ptr<osg::Texture2D> textureWall = new osg::Texture2D;
        const auto texturePath = (Core::g_runtimeContext.configManager->getMaterialFolder()/ "WoodPlanks.jpg").string();
        std::cout<<texturePath<<std::endl;
        auto imageWall = osgDB::readImageFile(texturePath);
        textureWall->setImage(imageWall);
        colorTextureVector.emplace_back(textureWall);

        //light projection matrix
        auto lightProjectionMatrix = depthPass->getProjectionMatrix();
        std::vector<osg::Matrixd*> lightMatrices;
        lightMatrices.emplace_back(&lightProjectionMatrix);

        Render::TextureProjectionPass *textureProjectionPass = new Render::TextureProjectionPass;
        textureProjectionPass->setGraphicsContext(graphicsContext);
        textureProjectionPass->setViewport(0,0,width,height);
        textureProjectionPass->setProjectionMatrixAsPerspective(60.0f, 1.0, 1.0f, 1000.0f);
        
        textureProjectionPass->setTextureArray(depthTextureVector, colorTextureVector, lightMatrices);
        viewer->addSlave(textureProjectionPass);

    };

    void tick(float deltaTime){
        
    };
    
private:
    //Camera 
    //Scene
    //Resource
    //pipeline
    std::shared_ptr<ResourceType::Level> m_levelResource;
    std::shared_ptr<ECS::Level> m_level;
    };
}