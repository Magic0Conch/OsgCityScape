#include "Component.h"
#include "osg/Node"
#include "osg/ref_ptr"

namespace CSEditor::ECS {

class Mesh:public Component{
public:
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;

private:
    osg::ref_ptr<osg::Node> meshNode;
};
}