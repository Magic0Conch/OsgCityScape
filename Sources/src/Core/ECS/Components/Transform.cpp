#include "Core/ECS/Components/Transform.h"
#include "Core/Helpers/Serializer.h"
#include "osg/Vec3f"

using namespace CSEditor::ECS;
using namespace CSEditor::Helpers;
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
}

void Transform::setRotation(const osg::Vec4f& rotation) {
    m_rotation = rotation;
}
void Transform::setScale(const osg::Vec3f& scale) {
    m_scale = scale;
}