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
#include "osg/Matrixd"
#include "osg/Texture2D"
#include "osg/Texture2DArray"
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

        const int width = 1200;
        const int height = 900;

        // initialize the level resource
        auto rootSceneNode = m_level->getRootObject()->getTransformComponent().getNode().get();
        rootSceneNode->setNodeMask(0x1);
        viewer->setSceneData(rootSceneNode);
        auto objects = m_levelResource->getObjects();
        for (const ResourceType::ObjectInstance& objectInstanceRes:objects) {
            auto curObjectId = m_level->loadObjectInstance(objectInstanceRes);
            auto thisTransform = m_level->getSceneObjectById(curObjectId)->getTransformComponent();
            m_level->getRootObject()->addChild(thisTransform);
        }
        m_level->setIsLoaded(true);

        //render pass
        //depth pass
        m_depthArray = new osg::Texture2DArray;
        m_depthArray->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        m_depthArray->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        m_depthArray->setInternalFormat(GL_DEPTH_COMPONENT);
        m_depthArray->setSourceFormat(GL_DEPTH_COMPONENT);
        m_depthArray->setSourceType(GL_FLOAT);
        m_depthArray->setTextureSize(width, height, 16);
        Render::RenderDepthToTexture *depthPass = new Render::RenderDepthToTexture;
        depthPass->setGraphicsContext(graphicsContext);
        depthPass->setNodeMask(0x1);
        depthPass->setViewport(0,0,width,height);
        depthPass->setProjectionMatrixAsPerspective(60.0f, 1.0, 0.1f, 5000.0f);
        depthPass->attach(osg::Camera::DEPTH_BUFFER,m_depthArray.get(),0,0);
        viewer->addSlave(depthPass);

        //color
        std::vector<osg::Texture2D *> colorTextureVector;
        osg::ref_ptr<osg::Texture2D> textureWall = new osg::Texture2D;
        const auto texturePath = (Core::g_runtimeContext.configManager->getMaterialFolder()/ "DJI_0314.JPG").string();
        std::cout<<texturePath<<std::endl;
        auto imageWall = osgDB::readImageFile(texturePath);
        textureWall->setImage(imageWall);
        colorTextureVector.emplace_back(textureWall);

        //light projection matrix
        auto lightViewMatrix = osg::Matrixd(0.695647,-0.319652,0.643348,0,0.718382,0.308087,-0.623706,0,0.001162,0.896050,0.443952,0,-3.889762,-81.353875,-186.377313,1);
        osg::Camera *lightCamera = new osg::Camera;
        lightCamera->setViewMatrix(lightViewMatrix);
        double fovy = 1.5500;
        double asectRatio = 1.333;
        double zNear = 5;
        double zFar = 1500;
        osg::Matrixd projectionMatrix = osg::Matrixd::perspective(fovy, asectRatio, zNear, zFar);        
        auto lightViewProjectionMatrix = lightViewMatrix*projectionMatrix;
        std::vector<osg::Matrixd*> lightMatrices;
        lightMatrices.emplace_back(&lightViewProjectionMatrix);

        auto mainCamera = Core::g_runtimeContext.windowSystem->getMainCamera();

        Render::TextureProjectionPass *textureProjectionPass = new Render::TextureProjectionPass(mainCamera);
        // mainCamera->setGraphicsContext(graphicsContext);
        // mainCamera->setViewport(0,0,width,height);
        mainCamera->setProjectionMatrixAsPerspective(60.0f, 1.0, 0.1f, 5000.0f);
        textureProjectionPass->setTextureArray(m_depthArray, colorTextureVector, lightMatrices);
        mainCamera->setNodeMask(0x1);
        // viewer->addSlave(textureProjectionPass);

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
    osg::ref_ptr<osg::Texture2DArray> m_depthArray;
    };
}