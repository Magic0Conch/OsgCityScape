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

class Shader:public Component{
public:
    Shader();

    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;
    

    void applyTo(osg::StateSet* stateSet);
    void setUniform();

private:
    
    std::vector<osg::Program> m_programs;

};
}
