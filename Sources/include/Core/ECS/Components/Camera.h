#pragma once
#include "Component.h"
#include "Core/ECS/Components/Transform.h"
#include "Render/LowRender/RenderDepthToTexture.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "osg/Camera"
#include "osg/CullSettings"
#include "osg/Matrixd"
#include "osg/Texture2D"
#include "osg/Vec3f"
#include "osg/ref_ptr"
#include <memory>
#include <string>
#include <vector>

namespace CSEditor::ECS {

class Camera : public Transform{
public:
    Camera();
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void onComponentAdded(std::shared_ptr<Object> parentObject) override;

    osg::ref_ptr<osg::Camera> getCameraNode() const;

    // Setter for m_cameraNode
    void setCameraNode(osg::ref_ptr<osg::Camera> node);

    void setFieldOfView(float fov);
    void setNearClippingPlane(float near_);
    void setFarClippingPlane(float far_);

    float getFieldOfView() const;
    float getNearClippingPlane() const;
    float getFarClippingPlane() const;
    std::vector<float> getPositionVector() const;
    std::vector<float> getRotationVector() const;

    virtual void setPosition(const osg::Vec3f& position) override;
    void setPositionVector(const std::vector<float> position);
    virtual void setRotation(const osg::Vec4f& rotation) override;
    virtual void setRotation(const osg::Quat& rotation) override;
    void setRotationVector(const std::vector<float> rotation);
    void setProjectionTexturePath(const std::string& path);
    void setProjectionTextureSize(int width, int height);
    std::string getProjectionTexturePath() const;

    void setRenderDepthToTexturePass(std::shared_ptr<Render::RenderDepthToTexture> renderDepthToTexture);
    void setIndexInProjectionPass(short index);
    void setTextureProjectionPass(std::shared_ptr<Render::TextureProjectionPass> textureProjectionPass);
    void setMatrixInRenderSystem();

    void setEnableProjectionTexture(bool enable);
    bool getEnableProjectionTexture() const;
    // void setPosition(const osg::Vec3d& position) {
    //     // m_cameraNode->setViewMatrixAsLookAt(position, m_cameraNode->getViewMatrix().getTrans(), m_cameraNode->getViewMatrix().getFluidUp());
    //     m_position = position;
    // }

    // void setRotation(const osg::Quat& rotation) {
    //     // osg::Vec3d eye, center, up;
    //     // m_cameraNode->getViewMatrixAsLookAt(eye, center, up);
    //     // m_cameraNode->setViewMatrixAsLookAt(eye, center, up * rotation);
    //     // m_rotation = rotation;
    // }

    // void setFieldOfView(double fov) {
    //     m_fov = fov;
    //     m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_cameraNode->getViewport()->width() / m_cameraNode->getViewport()->height(), m_near, m_far);
    // }

    // void setNearClippingPlane(double near_) {
    //     m_near = near_;
    //     m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_cameraNode->getViewport()->width() / m_cameraNode->getViewport()->height(), m_near, m_far);
    // }

    // void setFarClippingPlane(double far_) {
    //     m_far = far_;        
    //     m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_cameraNode->getViewport()->width() / m_cameraNode->getViewport()->height(), m_near, m_far);
    // }
    
    // const osg::Vec3d& getPosition() const {
    //     return m_position; 
    // }
    // const osg::Quat& getRotation() const { 
    //     return m_rotation;
    // }

    // double getFieldOfView() const { 
    //     return m_fov;
    // }

    // double getNearClippingPlane() const { 
    //     return m_near;
    // }

    // double getFarClippingPlane() const { 
    //     return m_far;
    // }

private:
    float m_fov = 65.657;
    float m_near;
    float m_far;
    float m_aspectRatio = 1.333333;
    void loadResourceAsync(std::shared_ptr<Object> parentObject);
    osg::ref_ptr<osg::Camera> m_cameraNode;
    
    std::string m_projectionTexturePath;
    float m_projectionTextureWidth = 1200;
    float m_projectionTextureHeight = 900;
    const osg::Vec3f m_defaultForward = osg::Vec3f(0, 0, -1);
    const osg::Vec3f m_defaultUp = osg::Vec3f(0, 1, 0);
    osg::Vec3 m_localForward = m_defaultForward;
    osg::Vec3 m_localUp = m_defaultUp;

    std::shared_ptr<Render::RenderDepthToTexture> m_renderDepthToTexture;
    short m_indexInProjectionPass = -1;
    std::shared_ptr<Render::TextureProjectionPass> m_textureProjectionPass;
    osg::Matrixd m_viewProjectionMatrix;
    bool m_enableProjectionTexture = true;
};

} // namespace CSEditor::ECS