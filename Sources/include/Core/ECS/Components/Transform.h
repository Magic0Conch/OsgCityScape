#include "Component.h"
#include "Core/ECS/Components/Component.h"
#include "osg/Vec3f"
#include "osg/Vec4f"

namespace CSEditor::ECS {

class Transform:public Component{
public:
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;

    osg::Vec3f getPosition() const;
    osg::Vec4f getRotation() const;
    osg::Vec3f getScale() const;

    void setPosition(const osg::Vec3f& position);
    void setRotation(const osg::Vec4f& rotation);
    void setScale(const osg::Vec3f& scale);
private:
    osg::Vec3f m_position;
    osg::Vec4f m_rotation;
    osg::Vec3f m_scale;
};
}
