#pragma once
#include "Core/ECS/Level.h"
#include "Editor/Core/RuntimeContext.h"
#include "Render/LowRender/RenderColorToTexture.h"
#include "Resources/ResourceType/Common/Level.h"
#include "osg/Camera"
#include "osg/Group"
#include "osg/ref_ptr"
#include "osgGA/TrackballManipulator"
#include "Windowing/Window.h"
#include "Core/ECS/WorldManager.h"
#include <memory>
#include <locale>
#include <iostream>
#include <osgDB/WriteFile>

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
        auto viewer = Core::g_runtimeContext.viewer;
        viewer->setSceneData(m_level->getRootObject()->getTransformComponent().getNode().get());
        // viewer
        // osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator();
        // manipulator->setHomePosition(osg::Vec3d(0,10,0), osg::Vec3d(0,0,0),osg::Vec3f(0,0,1));
        // viewer->setCameraManipulator(manipulator);

        auto objects = m_levelResource->getObjects();
        for (const ResourceType::ObjectInstance& objectInstanceRes:objects) {
            auto curObjectId = m_level->loadObjectInstance(objectInstanceRes);
            auto thisTransform = m_level->getSceneObjectById(curObjectId)->getTransformComponent();
            m_level->getRootObject()->addChild(thisTransform);
        }
        m_level->setIsLoaded(true);

        
        Render::RenderColorToTexture *rtt = new Render::RenderColorToTexture();
        rtt->setGraphicsContext(Core::g_runtimeContext.windowSystem->getGraphicsContext());
        rtt->setViewport(0,0,2048,2048);
        rtt->setProjectionMatrixAsPerspective(60.0f, 1.0, 1.0f, 1000.0f);
        Core::g_runtimeContext.viewer->addSlave(rtt);
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