#pragma once
#include "Component.h"
#include "osg/Geode"
#include "osg/Node"
#include "osg/ref_ptr"
#include <string>
#include <vector>

namespace CSEditor::ECS {

class Mesh:public Component{
public:
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void onComponentAdded(std::shared_ptr<Object> parentObject) override;
    Mesh() = default;

    osg::ref_ptr<osg::Geode> getMeshNode() const;
    void setMeshNode(osg::ref_ptr<osg::Geode> node);
    
protected:
    void commpressTexture();
    osg::ref_ptr<osg::Geode> m_meshNode;

};
}
