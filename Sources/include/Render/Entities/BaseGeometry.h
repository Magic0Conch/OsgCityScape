#ifndef BASEGEOMETRY_H
#define BASEGEOMETRY_H
#include <Windows.h>
#include "osg/Geometry"
namespace CSEditor::Render {
class BaseGeometry:public osg::Geometry{
private:
    virtual void updateMesh() = 0;
    class UpdateBaseGeometryCallback:public osg::NodeCallback{
    public:
        virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
    };
protected:
    bool isDirty = true;
    osg::ref_ptr<osg::DrawElementsUInt> indices;
    BaseGeometry(int segments);
public:
    int segments;
    void setSegments(float rhs);
};
}

#endif