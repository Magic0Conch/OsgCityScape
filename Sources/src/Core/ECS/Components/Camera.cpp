#include "Core/ECS/Components/Camera.h"
#include "Render/RenderSystem.h"
#include "Core/ECS/WorldManager.h"
#include "Editor/Core/RuntimeContext.h"
#include "osg/Matrixd"
#include "osg/Quat"
#include <osgDB/ReadFile>
#include "Core/Math/Math.h"
using namespace CSEditor::ECS;

Camera::Camera(){
    m_fov = 65.657;
    m_near = 5.0;
    m_far = 1500.0;
    m_position = osg::Vec3f(109.365,-42.4481,129.408);
    m_rotation = osg::Vec4f(-0.37530390976006250, -0.17732042570721485, -0.36462955593867941, 0.83351649653741111);
    osg::Quat orientation(m_rotation.x(), m_rotation.y(), m_rotation.z(), m_rotation.w());
    m_localForward = Math::Math::rotateVector(orientation, m_defaultForward);
    m_localUp = Math::Math::rotateVector(orientation, m_defaultUp);
}

void Camera::serialize(Json& jsonObject) {
    // 序列化逻辑
}

void Camera::deserialize(Json& jsonObject) {
    // 反序列化逻辑
}

void Camera::loadResource(std::shared_ptr<Object> parentObject) {
    m_cameraNode = new osg::Camera;
    m_parentObject = parentObject;
    m_cameraNode->setViewMatrixAsLookAt(m_position, m_position + m_localForward, m_localUp);
    m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_aspectRatio, m_near, m_far);
}

void Camera::onComponentAdded(std::shared_ptr<Object> parentObject) {
    loadResource(parentObject);
    auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
    auto ptr = std::dynamic_pointer_cast<Camera>(shared_from_this());
    activeLevel->addProjectionCamera(ptr);
    Core::g_runtimeContext.renderSystem->projectionCameraChanged();
}

osg::ref_ptr<osg::Camera> Camera::getCameraNode() const {
    return m_cameraNode;
}

void Camera::setCameraNode(osg::ref_ptr<osg::Camera> node) {
    m_cameraNode = node;
}

void Camera::setPositionVector(const std::vector<float> position){
    m_position = osg::Vec3f(position[0], position[1], position[2]);
    setMatrixInRenderSystem();
}

void Camera::setIndexInProjectionPass(short index){
    m_indexInProjectionPass = index;
}

void Camera::setRotationVector(const std::vector<float> rotation){
    m_rotation = osg::Vec4f(rotation[0], rotation[1], rotation[2], rotation[3]);
    osg::Quat quat(rotation[0], rotation[1], rotation[2], rotation[3]);
    m_localForward = Math::Math::rotateVector(quat, m_defaultForward);
    m_localUp = Math::Math::rotateVector(quat, m_defaultUp);
    setMatrixInRenderSystem();
}


void Camera::setPosition(const osg::Vec3f& position) {
    m_position = position;
    setMatrixInRenderSystem();
}

void Camera::setRotation(const osg::Quat& rotation) {
    m_rotation = osg::Vec4f(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    m_localForward = Math::Math::rotateVector(rotation, m_defaultForward);
    m_localUp = Math::Math::rotateVector(rotation, m_defaultUp);
    setMatrixInRenderSystem();
}

void Camera::setRotation(const osg::Vec4f& rotation) {
    m_rotation = rotation;
    osg::Quat quat(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    m_localForward = Math::Math::rotateVector(quat, m_defaultForward);
    m_localUp = Math::Math::rotateVector(quat, m_defaultUp);    
    setMatrixInRenderSystem();
}

void Camera::setFieldOfView(float fov) {
    m_fov = fov;
    setMatrixInRenderSystem();
}

void Camera::setNearClippingPlane(float near_) {
    m_near = near_;
    setMatrixInRenderSystem();
}

void Camera::setFarClippingPlane(float far_) {
    m_far = far_;
    setMatrixInRenderSystem();
}

void Camera::setProjectionTexturePath(const std::string& path){
    m_projectionTexturePath = path;
    auto projectionImage = osgDB::readImageFile(path);
    projectionImage->scaleImage(m_projectionTextureWidth, m_projectionTextureHeight, 1);    
    if(m_textureProjectionPass){
        m_textureProjectionPass->setTexture(m_indexInProjectionPass, projectionImage);
    }
}

void Camera::setProjectionTextureSize(int width, int height){
    m_projectionTextureWidth = width;
    m_projectionTextureHeight = height;
}

float Camera::getFieldOfView() const {
    return m_fov;
}

float Camera::getNearClippingPlane() const {
    return m_near;
}

float Camera::getFarClippingPlane() const {
    return m_far;
}

std::string Camera::getProjectionTexturePath() const{
    return m_projectionTexturePath;
}

std::vector<float> Camera::getPositionVector() const{
    return {m_position.x(), m_position.y(), m_position.z()};
}

std::vector<float> Camera::getRotationVector() const{
    return {m_rotation.x(), m_rotation.y(), m_rotation.z(), m_rotation.w()};
}

void Camera::setRenderDepthToTexturePass(std::shared_ptr<Render::DepthPass> renderDepthToTexture){
    m_renderDepthToTexture = renderDepthToTexture;
}

void Camera::setTextureProjectionPass(std::shared_ptr<Render::TextureProjectionPass> textureProjectionPass){
    m_textureProjectionPass = textureProjectionPass;
}

void Camera::setMatrixInRenderSystem(){
    m_cameraNode->setViewMatrixAsLookAt(m_position, m_position + m_localForward, m_localUp);
    m_cameraNode->setProjectionMatrixAsPerspective(m_fov, m_aspectRatio, m_near, m_far);
    m_viewProjectionMatrix =  m_cameraNode->getViewMatrix() * m_cameraNode->getProjectionMatrix();
    if(m_renderDepthToTexture){
        m_renderDepthToTexture->setViewMatrix(m_cameraNode->getViewMatrix());
        m_renderDepthToTexture->setProjectionMatrix(m_cameraNode->getProjectionMatrix());
    }
    if(m_textureProjectionPass){
        m_textureProjectionPass->setLightSpaceMatrixUniform(m_indexInProjectionPass,m_viewProjectionMatrix);
    }
}

void Camera::setEnableProjectionTexture(bool enable){
    m_enableProjectionTexture = enable;
    if(m_textureProjectionPass){
        m_textureProjectionPass->setProjectionEnabled(m_indexInProjectionPass, enable);
    }
}
bool Camera::getEnableProjectionTexture() const{
    return m_enableProjectionTexture;
}