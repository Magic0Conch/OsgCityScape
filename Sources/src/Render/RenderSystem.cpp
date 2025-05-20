#pragma once
#include "Render/RenderSystem.h"
#include "Core/ECS/WorldManager.h"
#include "Windowing/Window.h"
#include "Core/Math/MatrixHelper.h"
#include "Core/Math/Math.h"

namespace CSEditor::Render {
    RenderSystem::RenderSystem(){
        m_level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel(); 
        m_levelResource = m_level->getLevelResource();
        m_mainCamera = Core::g_runtimeContext.windowSystem->getMainCamera();       
        m_graphicsContext = Core::g_runtimeContext.windowSystem->getGraphicsContext(); 
        initialize();
    };

    void RenderSystem::setRenderPipelineState(const RenderPipelineState renderPipelineState){
        RenderConfig::getInstance().renderPipelineState = renderPipelineState;
        auto viewer = Core::g_runtimeContext.viewer;

        if(renderPipelineState == RenderPipelineState::Default){
            for (auto depthPass : m_depthPasses) {
                depthPass->setCullMask(0x1);
            }
            m_mainCamera->setCullMask(0x1);

            m_textureProjectionPass->setProjectionMatrix(*m_mainProjectionMatrix);
            m_textureProjectionPass->createTextureProjectionShader("TextureProjection");
        }
        else if (renderPipelineState == RenderPipelineState::TextureBaking) {
            for (auto depthPass : m_depthPasses) {
                depthPass->setCullMask(0x4);
            }
            m_mainCamera->setCullMask(0x4);

            auto scaleMatrix = osg::Matrix::scale(2,2,1);
            osg::Matrix translationMatrix = osg::Matrix::translate(-0.5, -0.5, 0.0);
            m_textureProjectionPass->setProjectionMatrix(translationMatrix*scaleMatrix);        
            m_textureProjectionPass->createTextureProjectionShader("UVUnwrap");
        }
    }

    void RenderSystem::projectionCameraChanged(){
        auto cameraVector = m_level->getCameras();
        m_lightMatrices.clear();
        m_depthPassVector.clear();
        auto viewer = Core::g_runtimeContext.viewer;
        int numSlaves = viewer->getNumSlaves();
        for (int i = numSlaves - 1; i >= 0; i--) {
            auto slaveCamera = viewer->getSlave(i)._camera;
            auto slaveCameraName = slaveCamera->getName();
            if (slaveCameraName == "DepthPass") {
                viewer->removeSlave(i);
            }
        }
        
        int index = 0;
        for(auto& camera : cameraVector){
            if(camera->getEnableProjectionTexture()){
                //matrix
                auto cameraNode = camera->getCameraNode();
                auto& cameraViewMatrix = cameraNode->getViewMatrix();
                auto& cameraProjectionMatrix = cameraNode->getProjectionMatrix();
                auto cameraViewProjectionMatrix = cameraViewMatrix * cameraProjectionMatrix;
                m_lightMatrices.emplace_back(cameraViewProjectionMatrix);
                //texture
                osg::Texture2D* projectionTexture = new osg::Texture2D;
                const auto texturePath = (Core::g_runtimeContext.configManager->getMaterialFolder()/ "DJI_0314.JPG").string();
                auto projectionImage = osgDB::readImageFile(texturePath);
                projectionImage->scaleImage(width, height, 1);
                projectionTexture->setImage(projectionImage);
                m_colorTextureVector.emplace_back(projectionTexture);     
                //depth
                auto depthPass =  std::make_shared<Render::DepthPass>();
                depthPass->setup(m_graphicsContext, width, height, m_depthArray, index, 0x1, 0);    
                depthPass->setViewMatrix(cameraViewMatrix);
                depthPass->setProjectionMatrix(cameraProjectionMatrix);
                camera->setRenderDepthToTexturePass(depthPass);
                camera->setIndexInProjectionPass(index);
                camera->setTextureProjectionPass(m_textureProjectionPass);
                Core::g_runtimeContext.viewer->addSlave(depthPass.get());
                m_depthPasses.emplace_back(depthPass);
                ++index;
            }
        }
        m_textureProjectionPass->setTextureArray(m_depthArray, m_colorTextureVector, m_lightMatrices);
    }

    void RenderSystem::createLightMatrices(){
        using namespace CSEditor::Math;
        float fovy = CSEditor::Math::Math::focal2fovEuler(1.55);
        auto viewMatrix = MatrixHelper::glmToOsgMatrix(glm::dmat4(0.671205,-0.474751,0.569293,0,0.740948,0.452385,-0.496331,0,-0.021905,0.754957,0.655409,0,-39.120093,-26.573260,-168.144351,1));
        m_mainCamera->setProjectionMatrixAsPerspective(fovy, 1.33333, 5, 1500);
        m_mainCamera->setViewMatrix(viewMatrix);

        m_mainProjectionMatrix = std::make_shared<osg::Matrixd>(m_mainCamera->getProjectionMatrix()) ;
        auto viewProjectionMatrix = viewMatrix * *m_mainProjectionMatrix;
        auto lightMatrix = viewMatrix * *m_mainProjectionMatrix;
    }

    void RenderSystem::createResources(){
        m_depthArray = new osg::Texture2DArray;
        m_depthArray->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        m_depthArray->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        m_depthArray->setInternalFormat(GL_DEPTH_COMPONENT);
        m_depthArray->setSourceFormat(GL_DEPTH_COMPONENT);
        m_depthArray->setSourceType(GL_FLOAT);
        m_depthArray->setTextureSize(width, height, 16);

        m_mainColorTexture = new osg::Texture2D();
        m_mainColorTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
        m_mainColorTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
        m_mainColorTexture->setSourceFormat(GL_RGBA);
        m_mainColorTexture->setInternalFormat(GL_RGBA8);
        m_mainColorTexture->setSourceType(GL_FLOAT);

        m_mainDepthStencilTexture = new osg::Texture2D();
        m_mainDepthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
        m_mainDepthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
        m_mainDepthStencilTexture->setSourceFormat(GL_DEPTH_STENCIL);
        m_mainDepthStencilTexture->setInternalFormat(GL_DEPTH24_STENCIL8);
        m_mainDepthStencilTexture->setSourceType(GL_UNSIGNED_INT_24_8);    
    }

    void RenderSystem::setupRenderPasses(){
        // Setup 
        auto viewer = Core::g_runtimeContext.viewer;
        auto mainViewport = Core::g_runtimeContext.windowSystem->getViewport();

        // Texture Projection Pass        
        m_textureProjectionPass = std::make_unique<Render::TextureProjectionPass>();
        m_textureProjectionPass->setup(m_mainCamera,m_mainColorTexture,m_mainDepthStencilTexture,0x1,1);
        
        m_captureCallback = new CaptureCallback(m_captureFlag,m_mainColorTexture);        
        m_mainCamera->setPreDrawCallback(m_captureCallback);

        // Opaque Effect Pass
        m_opaqueEffectPass = new OpaqueEffectPass;
        m_opaqueEffectPass->setup(m_graphicsContext, mainViewport, m_mainColorTexture, m_mainDepthStencilTexture, 0x2,2);
        m_opaqueEffectPass->setProjectionMatrix(*m_mainProjectionMatrix);
        viewer->addSlave(m_opaqueEffectPass);

        //Object Picker Pass
        m_objectPickerPass = std::make_unique<Render::ObjectPickerPass>(m_mainProjectionMatrix);
        setRenderPipelineState(RenderPipelineState::Default);
    }

    void RenderSystem::initialize(){
        createLightMatrices();
        createResources();
        setupRenderPasses();
    };

    void RenderSystem::tick(float deltaTime){
        m_objectPickerPass->highlightSelectedObject(RenderConfig::getInstance().renderPipelineState);
    };

    osg::Matrixd& RenderSystem::getMainProjectionMatrix(){
        return *m_mainProjectionMatrix;
    }

    void RenderSystem::captureFrame(){
        // std::string libraryName = "osgPlugins-3.6.4/osgdb_png"; // 例如加载PNG插件

        // if (osgDB::Registry::instance()->loadLibrary(libraryName)) {
        //     std::cout << "Successfully loaded library: " << libraryName << std::endl;
        // } else {
        //     std::cerr << "Failed to load library: " << libraryName << std::endl;
        // }

        m_captureCallback->directoryPath = Core::g_runtimeContext.configManager->getCaptureImageFolder().string();
        auto sceneObjects = m_level->getSceneObjectsMap();
        m_captureCallback->fileName = "SceneImage.jpg";
        *m_captureFlag = 0;
        
        // TaskQueue taskQueue;
        // for (const auto [id, objectPtr] : sceneObjects) {
        //     auto idCopy = id;
        //     auto objectPtrCopy = objectPtr;            
        //     taskQueue.pushTask([this,idCopy,objectPtrCopy]()
        //     {
        //         std::cout << "ID: " << idCopy << ", Object Pointer: " << objectPtrCopy << std::endl;
        //         auto mesh = objectPtrCopy->getComponent<ECS::ModelMesh>();
        //         if(mesh){
        //             while (!m_captureCallback->readyToCapture()) {
        //                 //忙等待
        //             }
        //             m_captureCallback->fileName = objectPtrCopy->getName() + ".jpg";
        //             auto& event = Core::g_runtimeContext.eventManager->getOrCreateEvent<Core::Event<int>>("SelectedObjectChanged");
        //             // onSelectedObjectChanged.reset(&event);
        //             event.invoke(idCopy);
        //             *m_captureFlag = 0;
        //         }                
        //     }
        //     );
        // }
        // std::thread taskThread([&taskQueue]() {
        //     taskQueue.processTasks();
        // });
        // taskThread.detach();
    }
    
    std::shared_ptr<int> RenderSystem::getCaptureFlag(){
        return m_captureFlag;
    }
}