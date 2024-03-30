#pragma once
#include "Component.h"
#include "osg/Camera"
#include "osg/CullSettings"
#include "osg/ref_ptr"
#include <string>

namespace CSEditor::ECS {

class Camera : public Component {
public:
    Camera();
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void onComponentAdded() override;

    osg::ref_ptr<osg::Camera> getCameraNode() const;

    // Setter for m_cameraNode
    void setCameraNode(osg::ref_ptr<osg::Camera> node);
    void setPosition(const osg::Vec3d& position);
    void setRotation(const osg::Quat& rotation);
    void setFieldOfView(double fov);
    void setNearClippingPlane(double near_);
    void setFarClippingPlane(double far_);

    const osg::Vec3d& getPosition() const;
    const osg::Quat& getRotation() const;
    double getFieldOfView() const;
    double getNearClippingPlane() const;
    double getFarClippingPlane() const;

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
    void loadResourceAsync(std::shared_ptr<Object> parentObject);
    osg::ref_ptr<osg::Camera> m_cameraNode;

    osg::Vec3d m_position;
    osg::Quat m_rotation;
    
    double m_fov;
    double m_near;
    double m_far;
};

} // namespace CSEditor::ECS