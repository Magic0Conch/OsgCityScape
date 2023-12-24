#pragma once
#include "osg/Vec3f"
#include <vector>
namespace CSEditor::ResourceType {

class Level{
public:


private:
    osg::Vec3f m_gravity {0.f, 0.f, -9.8f};

    // std::vector<ObjectInstance> m_objects;
};
}

