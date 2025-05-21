#pragma once
#include "Render/RenderSystem.h"
#include "Core/ECS/WorldManager.h"
#include "Render/Pass/ImageProjectionPass.h"
#include "Windowing/Window.h"
#include "Core/Math/MatrixHelper.h"
#include "Core/Math/Math.h"
#include "osg/ref_ptr"
#include <iostream>

namespace CSEditor::Render {


    RenderSystem::RenderSystem(){
        m_level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel(); 
        m_levelResource = m_level->getLevelResource();
        m_mainCamera = Core::g_runtimeContext.windowSystem->getMainCamera();       
        m_graphicsContext = Core::g_runtimeContext.windowSystem->getGraphicsContext(); 
        initialize();
    };

    void RenderSystem::deleteSlaveCameras(){
        auto viewer = Core::g_runtimeContext.viewer;
        // Step 1.1: 收集所有待删索引（或指针）
        std::vector<int> toDelete;
        for (int i = 0; i < viewer->getNumSlaves(); ++i) {
            auto slaveCamera = viewer->getSlave(i)._camera;
            if (slaveCamera && (slaveCamera->getName() == "DepthPass"||slaveCamera->getName() == "TextureProjection")) {
                toDelete.push_back(i);
            }
        }
        // Step 1.2: 统一倒序删
        for (int i = toDelete.size() - 1; i >= 0; --i) {
            viewer->removeSlave(toDelete[i]);
        }
        m_colorTextureVector.clear();
        m_depthPasses.clear();
        m_lightMatrices.clear();
        m_depthArray.clear();
    }
    void RenderSystem::setRenderPipelineState(const RenderPipelineState renderPipelineState){
        RenderConfig::getInstance().renderPipelineState = renderPipelineState;
        auto viewer = Core::g_runtimeContext.viewer;

        if(renderPipelineState == RenderPipelineState::Default){
            for (auto depthPass : m_depthPasses) {
                depthPass->setCullMask(0x1);
            }
            m_mainCamera->setCullMask(0x1);
            m_mainCamera->setClearColor(osg::Vec4f(0.529f, 0.808f, 0.922f,1.0f));
            // m_textureProjectionPass->setProjectionMatrix(*m_mainProjectionMatrix);
            // m_textureProjectionPass->createTextureProjectionShader("TextureProjection");
        }
        else if (renderPipelineState == RenderPipelineState::TextureBaking) {
            for (auto depthPass : m_depthPasses) {
                depthPass->setCullMask(0x4);
            }
            m_mainCamera->setCullMask(0x4);
            m_mainCamera->setClearColor(osg::Vec4f(0,0,0,0));
            auto scaleMatrix = osg::Matrix::scale(2,2,1);
            osg::Matrix translationMatrix = osg::Matrix::translate(-0.5, -0.5, 0.0);
            // m_textureProjectionPass->setProjectionMatrix(translationMatrix*scaleMatrix);        
            // m_textureProjectionPass->createTextureProjectionShader("UVUnwrap");
        }
    }

    void RenderSystem::projectionCameraChanged(){
        auto cameraVector = m_level->getCameras();
        m_lightMatrices.clear();
        m_depthPassVector.clear();
        auto viewer = Core::g_runtimeContext.viewer;
        deleteSlaveCameras();
        
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

                osg::ref_ptr<osg::Texture2D> depthMap = new osg::Texture2D;
                depthMap->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
                depthMap->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
                depthMap->setInternalFormat(GL_DEPTH_COMPONENT);
                depthMap->setSourceFormat(GL_DEPTH_COMPONENT);
                depthMap->setSourceType(GL_FLOAT);
                depthMap->setTextureSize(width, height);

                // auto projectorColorTexture = new osg::Texture2D();
                // projectorColorTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
                // projectorColorTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
                // projectorColorTexture->setSourceFormat(GL_RGBA);
                // projectorColorTexture->setInternalFormat(GL_RGBA8);
                // projectorColorTexture->setSourceType(GL_FLOAT);

                // auto projectorDepthStencilTexture = new osg::Texture2D();
                // projectorDepthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
                // projectorDepthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
                // projectorDepthStencilTexture->setSourceFormat(GL_DEPTH_STENCIL);
                // projectorDepthStencilTexture->setInternalFormat(GL_DEPTH24_STENCIL8);
                // projectorDepthStencilTexture->setSourceType(GL_UNSIGNED_INT_24_8);    

                //depth
                osg::ref_ptr<Render::DepthPass> depthPass =  new Render::DepthPass;
                depthPass->setup(m_graphicsContext, width, height, depthMap, 0x1, index);    
                depthPass->setViewMatrix(cameraViewMatrix);
                depthPass->setProjectionMatrix(cameraProjectionMatrix);
                camera->setRenderDepthToTexturePass(depthPass);
                camera->setIndexInProjectionPass(index);
                Core::g_runtimeContext.viewer->addSlave(depthPass.get());
                m_depthPasses.emplace_back(depthPass);
                m_depthArray.emplace_back(depthMap);

                // Texture Projection
                bool pingpong = index&1;
                osg::ref_ptr<Render::ImageProjectionPass> imageProjectionPass = new ImageProjectionPass;
                
                imageProjectionPass->setup(m_graphicsContext, width, height,pingpong?m_projectorColorTexture:m_mainColorTexture, projectionTexture,depthMap ,m_lightMatrices[0],m_projectorDepthStencilTexture,pingpong? m_mainColorTexture:m_projectorColorTexture,0x1,index+30);
                imageProjectionPass->setProjectionMatrix(*m_mainProjectionMatrix);
                // imageProjectionPass->createTextureProjectionShader("ImageProjection");
                
                m_imageProjectionPasses.emplace_back(imageProjectionPass);
                Core::g_runtimeContext.viewer->addSlave(imageProjectionPass.get());
                camera->setTextureProjectionPass(imageProjectionPass);
                ++index;
            }
        }
        // m_textureProjectionPass->setTextureArray(m_depthArray[0], m_colorTextureVector, m_lightMatrices);
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

        m_projectorColorTexture = new osg::Texture2D();
        m_projectorColorTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
        m_projectorColorTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
        m_projectorColorTexture->setSourceFormat(GL_RGBA);
        m_projectorColorTexture->setInternalFormat(GL_RGBA8);
        m_projectorColorTexture->setSourceType(GL_FLOAT);

        m_projectorDepthStencilTexture = new osg::Texture2D();
        m_projectorDepthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
        m_projectorDepthStencilTexture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
        m_projectorDepthStencilTexture->setSourceFormat(GL_DEPTH_STENCIL);
        m_projectorDepthStencilTexture->setInternalFormat(GL_DEPTH24_STENCIL8);
        m_projectorDepthStencilTexture->setSourceType(GL_UNSIGNED_INT_24_8);    
    }

    void RenderSystem::setupRenderPasses(){
        // Setup 
        auto viewer = Core::g_runtimeContext.viewer;
        auto mainViewport = Core::g_runtimeContext.windowSystem->getViewport();

        //triangle
        m_trianglePass = new Render::TrianglePass;
        m_trianglePass->setup(m_graphicsContext,mainViewport->width(),mainViewport->height(),m_mainColorTexture,m_mainDepthStencilTexture,0x1,10);
        m_trianglePass->setProjectionMatrix(*m_mainProjectionMatrix);
        viewer->addSlave(m_trianglePass);


        // Texture Projection Pass        
        // m_textureProjectionPass = std::make_unique<Render::TextureProjectionPass>();
        // m_textureProjectionPass->setup(m_mainCamera,m_mainColorTexture,m_mainDepthStencilTexture,0x1,60);
        

        // Opaque Effect Pass
        m_opaqueEffectPass = new OpaqueEffectPass;
        m_opaqueEffectPass->setup(m_graphicsContext, mainViewport, m_mainColorTexture, m_mainDepthStencilTexture, 0x2,90);
        m_opaqueEffectPass->setProjectionMatrix(*m_mainProjectionMatrix);
        viewer->addSlave(m_opaqueEffectPass);

        //Object Picker Pass
        m_objectPickerPass = std::make_unique<Render::ObjectPickerPass>(m_mainProjectionMatrix);
        setRenderPipelineState(RenderPipelineState::Default);

        m_mainCamera->setCullMask(0x1);
        m_captureCallback = new CaptureCallback(m_captureFlag,m_mainColorTexture);        
        m_mainCamera->setPreDrawCallback(m_captureCallback);

        m_displayPass = new TextureDisplayPass();
        m_displayPass->setup(m_graphicsContext, mainViewport->width(),mainViewport->height(), m_mainColorTexture, 999); // 999保证最后显示
        viewer->addSlave(m_displayPass,false);
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
        *m_captureFlag = 0;
    }
    
    std::shared_ptr<int> RenderSystem::getCaptureFlag(){
        return m_captureFlag;
    }
}