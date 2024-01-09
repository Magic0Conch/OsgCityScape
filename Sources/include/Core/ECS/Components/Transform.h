#pragma once
#include "Component.h"
#include "Core/ECS/Components/Component.h"
#include "osg/NodeVisitor"
#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/ref_ptr"
#include <string>
#include <osg/PositionAttitudeTransform>

namespace CSEditor::ECS {

class Transform:public Component{
public:
    Transform();

    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;
    
    osg::Vec3f getPosition() const;
    osg::Vec4f getRotation() const;
    osg::Vec3f getScale() const;
    osg::ref_ptr<osg::PositionAttitudeTransform> getNode();

    void setPosition(const osg::Vec3f& position);
    void setRotation(const osg::Vec4f& rotation);
    void setScale(const osg::Vec3f& scale);
    void setNode(osg::ref_ptr<osg::PositionAttitudeTransform> node);
private:
    osg::Vec3f m_position;
    osg::Vec4f m_rotation;
    osg::Vec3f m_scale;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_node;
    
};
}
