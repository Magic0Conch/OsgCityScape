#pragma once
#include "Component.h"
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
    virtual void onComponentAdded() override;

    osg::ref_ptr<osg::Node> getMeshNode() const;

    // Setter for m_meshNode
    void setMeshNode(osg::ref_ptr<osg::Node> node);

    void setMeshPath(const std::string& meshPath);
    const std::string& getMeshPath();
    std::string m_meshPath;
private:
    osg::ref_ptr<osg::Node> m_meshNode;
    std::vector<std::string> m_materialPaths;
};
}
