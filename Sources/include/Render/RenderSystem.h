#pragma once
#include "Core/ECS/Level.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceType/Common/Level.h"
#include "osg/ref_ptr"
#include "osgGA/TrackballManipulator"
#include "Core/ECS/WorldManager.h"
#include <memory>
namespace CSEditor::Render {
class RenderSystem{
public:
    RenderSystem(){
        m_level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
        m_levelResource = m_level->getLevelResource();
        initialize();
    };

    ~RenderSystem(){

    };

    void initialize(){
        auto viewer = Core::g_runtimeContext.viewer;
        viewer->setSceneData(m_level->getRootObject()->getTransformComponent().getNode().get());
        // viewer
        osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator();
        manipulator->setHomePosition(osg::Vec3d(0,10,0), osg::Vec3d(0,0,0),osg::Vec3f(0,0,1));
        viewer->setCameraManipulator(manipulator);

        auto objects = m_levelResource->getObjects();
        for (const ResourceType::ObjectInstance& objectInstanceRes:objects) {
            auto curObjectId = m_level->loadObjectInstance(objectInstanceRes);
            auto thisTransform = m_level->getSceneObjectById(curObjectId)->getTransformComponent();
            m_level->getRootObject()->addChild(thisTransform);
        }
        m_level->setIsLoaded(true);
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