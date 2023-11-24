#ifndef BASEGEOMETRY_H
#define BASEGEOMETRY_H
#include <Windows.h>
#include "osg/Geometry"
#include "osg/RenderInfo"
namespace CSEditor::Render {
class BaseGeometry:public osg::Geometry{
public:
    BaseGeometry(int segments);
    void setSegments(float rhs);
protected:
    bool m_isDirty = true;
    int m_segments;
    osg::ref_ptr<osg::DrawElementsUInt> m_indices;
    void drawImplementation(osg::RenderInfo& renderInfo) const override;
private:
    virtual void update() = 0;
    class UpdateBaseGeometryCallback:public osg::NodeCallback{
    public:
        virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
    };
};
}

#endif