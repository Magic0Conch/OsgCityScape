#include "Core/ECS/Components/Camera.h"

using namespace CSEditor::ECS;

Camera::Camera(){
    m_fov = 60.0;
    m_near = 1.0;
    m_far = 1000.0;
}

void Camera::serialize(Json& jsonObject) {
    // 序列化逻辑
}

void Camera::deserialize(Json& jsonObject) {
    // 反序列化逻辑
}

void Camera::loadResource(std::shared_ptr<Object> parentObject) {
    // 加载资源逻辑
}

void Camera::onComponentAdded() {
    // 组件添加后的逻辑
}

osg::ref_ptr<osg::Camera> Camera::getCameraNode() const {
    return m_cameraNode;
}

void Camera::setCameraNode(osg::ref_ptr<osg::Camera> node) {
    m_cameraNode = node;
}

void Camera::setPosition(const osg::Vec3d& position) {
    // m_position = position;
    // m_cameraNode->setViewMatrixAsLookAt(m_position, m_cameraNode->getViewMatrix().getTrans(), m_cameraNode->getViewMatrix().getFluidUp());
}

void Camera::setRotation(const osg::Quat& rotation) {
    // m_rotation = rotation;
    // osg::Vec3d eye, center, up;
    // m_cameraNode->getViewMatrixAsLookAt(eye, center, up);
    // m_cameraNode->setViewMatrixAsLookAt(eye, center, up * rotation);
}

void Camera::setFieldOfView(double fov) {
    m_fov = fov;
    m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_cameraNode->getViewport()->width() / m_cameraNode->getViewport()->height(), m_near, m_far);
}

void Camera::setNearClippingPlane(double near_) {
    m_near = near_;
    m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_cameraNode->getViewport()->width() / m_cameraNode->getViewport()->height(), m_near, m_far);
}

void Camera::setFarClippingPlane(double far_) {
    m_far = far_;
    m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_cameraNode->getViewport()->width() / m_cameraNode->getViewport()->height(), m_near, m_far);
}

const osg::Vec3d& Camera::getPosition() const {
    return m_position;
}

const osg::Quat& Camera::getRotation() const {
    return m_rotation;
}

double Camera::getFieldOfView() const {
    return m_fov;
}

double Camera::getNearClippingPlane() const {
    return m_near;
}

double Camera::getFarClippingPlane() const {
    return m_far;
}