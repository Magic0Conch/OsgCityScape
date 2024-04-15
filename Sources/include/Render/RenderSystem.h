#pragma once
#include "Core/ECS/Level.h"
#include "Core/Math/MatrixHelper.h"
#include "Editor/Core/RuntimeContext.h"
#include "Render/LowRender/DisplayTexture.h"
#include "Render/LowRender/RenderColorToTexture.h"
#include "Render/LowRender/RenderDepthToTexture.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Resources/ResourceType/Common/Level.h"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
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
#include "Core/Math/MatrixHelper.h"
#include <osg/CullFace>
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
        auto camera = Core::g_runtimeContext.windowSystem->getMainCamera();
        osg::Vec3d eye, center, up;
        auto viewMatrix = MatrixHelper::glmToOsgMatrix(glm::dmat4(0.671205,-0.474751,0.569293,0,0.740948,0.452385,-0.496331,0,-0.021905,0.754957,0.655409,0,-39.120093,-26.573260,-168.144351,1));
        auto perspectiveMatrix = MatrixHelper::glmToOsgMatrix(MatrixHelper::getPerspectiveMatrix(1.55, 5, 1500, 1.0));        
        camera->setViewMatrix(viewMatrix);
        camera->setProjectionMatrix(perspectiveMatrix);
        auto viewProjectionMatrix = viewMatrix * perspectiveMatrix;
        camera->getViewMatrixAsLookAt(eye, center, up);
        osg::Matrixd rotationMatrix(1, 0, 0, 0,
                                    0, 0, -1, 0,
                                    0, 1, 0, 0,
                                    0, 0, 0, 1);
        auto lightMatrix = rotationMatrix * viewMatrix * perspectiveMatrix;
        
        //setup 
        auto mainCamera = Core::g_runtimeContext.windowSystem->getMainCamera();
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

        m_depthPass = new Render::RenderDepthToTexture;
        m_depthPass->setGraphicsContext(graphicsContext);
        m_depthPass->setNodeMask(0x1);
        m_depthPass->setViewport(0,0,width,height);
        m_depthPass->setViewProjectionMatrix(lightMatrix);
        m_depthPass->attach(osg::Camera::DEPTH_BUFFER,m_depthArray.get(),0,0);
        viewer->addSlave(m_depthPass);

        //color
        std::vector<osg::Texture2D *> colorTextureVector;
        osg::ref_ptr<osg::Texture2D> textureWall = new osg::Texture2D;
        const auto texturePath = (Core::g_runtimeContext.configManager->getMaterialFolder()/ "DJI_0314.JPG").string();
        auto imageWall = osgDB::readImageFile(texturePath);
        textureWall->setImage(imageWall);
        colorTextureVector.emplace_back(textureWall);

        // light projection matrix
        m_lightMatrices.emplace_back(&lightMatrix);


        m_textureProjectionPass = std::make_unique<Render::TextureProjectionPass>(mainCamera);
        // mainCamera->setGraphicsContext(graphicsContext);
        // mainCamera->setViewport(0,0,width,height);
        // mainCamera->setProjectionMatrixAsPerspective(60.0f, 1.0, 0.1f, 5000.0f);        
        m_textureProjectionPass->setTextureArray(m_depthArray, colorTextureVector, m_lightMatrices);
        mainCamera->setNodeMask(0x1);
        mainCamera->setCullingMode(mainCamera->getCullingMode() & ~osg::CullSettings::SMALL_FEATURE_CULLING);
        osg::ref_ptr<osg::CullFace> cullFace = new osg::CullFace;
        cullFace->setMode(osg::CullFace::BACK);
        mainCamera->getOrCreateStateSet()->setAttributeAndModes(cullFace, osg::StateAttribute::ON);


        // viewer->addSlave(textureProjectionPass);

    };

    void tick(float deltaTime){
        auto mainCamera = Core::g_runtimeContext.windowSystem->getMainCamera();
        auto viewMatrix = mainCamera->getProjectionMatrix() * mainCamera->getViewMatrix();
        // m_lightMatrices[0] = &viewMatrix;
        
    };
    
private:
    //Camera 
    //Scene
    //Resource
    //pipeline
    std::shared_ptr<ResourceType::Level> m_levelResource;
    std::shared_ptr<ECS::Level> m_level;
    osg::ref_ptr<osg::Texture2DArray> m_depthArray;
    std::vector<osg::Matrixd*> m_lightMatrices;
    osg::ref_ptr<Render::RenderDepthToTexture> m_depthPass;
    std::unique_ptr<Render::TextureProjectionPass> m_textureProjectionPass;
};
}