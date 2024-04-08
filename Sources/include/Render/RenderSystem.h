#pragma once
#include "Core/ECS/Level.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceType/Common/Level.h"
#include "osg/ref_ptr"
#include "osgGA/TrackballManipulator"
#include "Core/ECS/WorldManager.h"
#include <memory>
#include <locale>
#include <iostream>
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


        osg::ref_ptr<osg::Texture2D> depthTexture = new osg::Texture2D;
        depthTexture->setTextureSize(1024, 1024);
        depthTexture->setInternalFormat(GL_DEPTH_COMPONENT);
        depthTexture->setSourceFormat(GL_DEPTH_COMPONENT);
        depthTexture->setSourceType(GL_FLOAT);

        osg::ref_ptr<osg::Camera> depthCamera = new osg::Camera;
        depthCamera->setClearMask(GL_DEPTH_BUFFER_BIT); // 只清除深度缓冲
        depthCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        depthCamera->setRenderOrder(osg::Camera::PRE_RENDER); // 在渲染主场景之前渲染
        depthCamera->setViewport(0, 0, 1024, 1024);
        depthCamera->attach(osg::Camera::DEPTH_BUFFER, depthTexture.get()); // 将深度缓冲附加到纹理


        osg::ref_ptr<osg::Group> root = new osg::Group;
        root->addChild(depthCamera.get());

        // depthCamera->render();


        // osgDB::writeImageFile(*depthTexture->getImage(0), "depth.png");
        
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