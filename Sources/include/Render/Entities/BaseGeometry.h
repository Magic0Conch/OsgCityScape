#ifndef BASEGEOMETRY_H
#define BASEGEOMETRY_H
#include <Windows.h>
#include "osg/Geometry"
namespace CSEditor::Render {
class BaseGeometry:public osg::Geometry{
public:
    BaseGeometry(int segments);
    void setSegments(float rhs);
private:
    virtual void update() = 0;
    class UpdateBaseGeometryCallback:public osg::NodeCallback{
    public:
        virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
    };
protected:
    bool m_isDirty = true;
    int m_segments;
    osg::ref_ptr<osg::DrawElementsUInt> m_indices;
};
}

#endif