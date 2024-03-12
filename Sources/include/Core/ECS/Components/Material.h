#pragma once
#include "Component.h"
#include "Core/ECS/Components/Component.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "osg/NodeVisitor"
#include "osg/StateSet"
#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/ref_ptr"
#include <osg/PositionAttitudeTransform>
#include <vector>

namespace CSEditor::ECS {

class Material:public Component{
public:
    Material();

    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;
    
        
    void setNode(osg::ref_ptr<osg::PositionAttitudeTransform> node);
    std::shared_ptr<Object> getObject() const;
    ObjectID getObjectID() const;
    const bool isLeaf() const;
    const std::vector<int>& getChildIndex() const; 

private:
    osg::ref_ptr<osg::StateSet> m_stateSet;
    std::vector<int> m_childrenIndex;
};
}
