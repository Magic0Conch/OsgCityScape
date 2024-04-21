#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/Object.h"
#include "Core/Helpers/Serializer.h"
#include "osg/Quat"
#include "osg/Vec3f"
#include "spdlog/spdlog.h"
#include <memory>
#include "Core/ECS/Level.h"
#include "Core/ECS/WorldManager.h"
#include "Editor/Core/RuntimeContext.h"

using namespace CSEditor::ECS;
using namespace CSEditor::Helpers;

Transform::Transform(){
}

void Transform::serialize(Json& jsonObject){

}

void Transform::deserialize(Json& jsonObject){
    auto positionJson = jsonObject["position"];
    auto rotationJson = jsonObject["rotation"];
    auto scaleJson = jsonObject["scale"];
    osg::Vec3f position;
    osg::Vec4f rotation;
    osg::Vec3f scale;
    Serializer::deserializeVector3f(positionJson, position);
    Serializer::deserializeVector4f(rotationJson, rotation);
    Serializer::deserializeVector3f(scaleJson, scale);
    m_position = position;
    m_rotation = rotation;
    m_scale = scale;
}

osg::Vec3f Transform::getPosition() const{
    return m_position;
}
osg::Vec4f Transform::getRotation() const {
    return m_rotation;
}

osg::Vec3f Transform::getScale() const {
    return m_scale;
}
void Transform::setPosition(const osg::Vec3f& position) {
    m_position = position;
    m_node->setPosition(m_position);
}

void Transform::setRotation(const osg::Vec4f& rotation) {
    m_rotation = rotation;
    m_node->setAttitude(osg::Quat(m_rotation));
}

void Transform::setRotation(const osg::Quat& rotation) {    
    m_node->setAttitude(rotation);
}

void Transform::setScale(const osg::Vec3f& scale) {
    m_scale = scale;
    m_node->setScale(m_scale);
}

osg::ref_ptr<osg::PositionAttitudeTransform> Transform::getNode(){
    return m_node;
}
void Transform::setNode(osg::ref_ptr<osg::PositionAttitudeTransform> node){
    m_node = node;
    CSEditor::Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->nodeToObjectID[m_node] = m_parentObject.lock();
}

void Transform::loadResource(std::shared_ptr<Object> parentObject){
    m_node = new osg::PositionAttitudeTransform();
    m_parentObject = parentObject;
    m_node->setPosition(m_position);
    m_node->setAttitude(osg::Quat(m_rotation)); 
    m_node->setScale(m_scale);
    onComponentAdded();
}

void Transform::onComponentAdded(){
    if(Core::g_runtimeContext.worldManager==nullptr||m_parentObject.lock() == nullptr)
        return;
    auto level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
    if(level->getIsLoaded()){
        level->nodeToObjectID[m_node] = m_parentObject.lock();
    }
}

void Transform::addChild(Transform& childTransform){
    m_childrenIndex.emplace_back(childTransform.getObjectID());
    getNode()->addChild(childTransform.getNode());
}

std::shared_ptr<Object> Transform::getObject() const{
    return m_parentObject.lock();
}

ObjectID Transform::getObjectID() const{
    return m_parentObject.lock()->getID();
}

const bool Transform::isLeaf() const{
    return m_childrenIndex.empty();
}

const std::vector<int>& Transform::getChildIndex() const{
    return m_childrenIndex;
}