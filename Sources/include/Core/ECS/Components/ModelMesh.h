#pragma once
#include "Component.h"
#include "Core/ECS/Components/Mesh.h"
#include "osg/Node"
#include "osg/ref_ptr"
#include <string>
#include <vector>

namespace CSEditor::ECS {

class ModelMesh:public Mesh{
public:
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void onComponentAdded(std::shared_ptr<Object> parentObject) override;
    ModelMesh() = default;
    ModelMesh(std::string meshPath);
    void loadResourceAsync(std::shared_ptr<Object> parentObject);
    void setMeshPath(const std::string& meshPath);
    const std::string& getMeshPath();
    std::string m_meshPath;

private:
    std::vector<std::string> m_materialPaths;
};
}
