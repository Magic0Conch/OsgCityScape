#pragma once
#include "osg/Vec3f"
#include "Core/Helpers/ISerializer.h"
#include "Resources/ResourceType/Common/Object.h"
#include <vector>
namespace CSEditor::ResourceType {

class Level: public Helpers::ISerializable{
public:
    osg::Vec3f getGravity() const ;

    void setGravity(const osg::Vec3f& gravity);
    void setGravity(const float x,const float y,const float z);

    std::vector<ObjectInstance> getObjects() const;

    void setObjects(const std::vector<ObjectInstance>& objects);
    void addObject(const ObjectInstance& object);

    virtual void serialize(Json& jsonBody) override;
    virtual void deserialize(Json& jsonBody) override;
    void loadResource();

private:
    osg::Vec3f m_gravity {0.f, 0.f, -9.8f};
    std::vector<ObjectInstance> m_objects;
    // std::vector<ObjectInstance> m_objects;
};
}

