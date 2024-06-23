#pragma once
#include "Component.h"
#include "Core/ECS/Components/Component.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "osg/NodeVisitor"
#include "osg/Quat"
#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/ref_ptr"
#include <osg/PositionAttitudeTransform>
#include <vector>

namespace CSEditor::ECS {

class Transform:public Component, public std::enable_shared_from_this<Transform>{
public:
    Transform();

    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;
    virtual void onComponentAdded(std::shared_ptr<Object> parentObject) override;

    osg::Vec3f getPosition() const;
    osg::Vec4f getRotation() const;
    osg::Vec3f getScale() const;
    osg::ref_ptr<osg::PositionAttitudeTransform> getNode();

    virtual void setPosition(const osg::Vec3f& position);
    virtual void setRotation(const osg::Vec4f& rotation);
    virtual void setRotation(const osg::Quat& rotation);
    void setScale(const osg::Vec3f& scale);
    void setNode(osg::ref_ptr<osg::PositionAttitudeTransform> node);
    void addChild(Transform& childTransform);
    std::shared_ptr<Object> getObject() const;
    ObjectID getObjectID() const;
    const bool isLeaf() const;
    const std::vector<int>& getChildIndex() const; 

protected:
    osg::Vec3f m_position = osg::Vec3f(0,0,0);
    osg::Vec4f m_rotation = osg::Vec4f(0,0,0,1);
    osg::Vec3f m_scale = osg::Vec3f(1,1,1);
    osg::ref_ptr<osg::PositionAttitudeTransform> m_node;
    std::vector<int> m_childrenIndex;
};
}
