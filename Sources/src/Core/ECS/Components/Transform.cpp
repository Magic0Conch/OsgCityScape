#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/Object.h"
#include "Core/Helpers/Serializer.h"
#include "osg/Quat"
#include "osg/Vec3f"
#include <memory>

using namespace CSEditor::ECS;
using namespace CSEditor::Helpers;

Transform::Transform(){
    m_node = new osg::PositionAttitudeTransform();
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
    setPosition(position);
    setRotation(rotation);
    setScale(scale);
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
void Transform::setScale(const osg::Vec3f& scale) {
    m_scale = scale;
    m_node->setScale(m_scale);
}

osg::ref_ptr<osg::PositionAttitudeTransform> Transform::getNode(){
    return m_node;
}
void Transform::setNode(osg::ref_ptr<osg::PositionAttitudeTransform> node){
    m_node = node;
}

void Transform::loadResource(std::shared_ptr<Object> parentObject){
    m_parentObject = parentObject;
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