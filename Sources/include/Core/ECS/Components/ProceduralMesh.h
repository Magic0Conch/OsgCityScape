#pragma once
#include "Component.h"
#include "Core/ECS/Components/Material.h"
#include "Core/ECS/Components/Mesh.h"
#include "Render/Entities/BaseGeometry.h"
#include "osg/Node"
#include "osg/ref_ptr"
#include <memory>
#include <string>
#include <vector>

namespace CSEditor::ECS {

class ProceduralMesh:public Mesh{
public:
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void onComponentAdded(std::shared_ptr<Object> parentObject) override;
    ProceduralMesh() = default;
    
    void addGeometry(osg::ref_ptr<Render::BaseGeometry> geometry, std::shared_ptr<Material> material);    
    std::vector<osg::ref_ptr<Render::BaseGeometry>>& getGeometries();
    std::vector<std::shared_ptr<Material>>& getMaterials();
protected:
    std::vector<osg::ref_ptr<Render::BaseGeometry>> m_geometries;
    std::vector<std::shared_ptr<Material>> m_materials;
};
}
