#ifndef GeometryFactory_H
#define GeometryFactory_H
#include <windows.h>
#include <osg/Geometry>
#include <osg/ref_ptr>
namespace CSEditor::Render{
enum class BaseGeometryType{
    QUAD,Cylinder,Circle
};

class BaseGeometryFactory{
public:
    static osg::ref_ptr<osg::Geometry> createBaseGeometry(const BaseGeometryType );
    static osg::ref_ptr<osg::Geometry> createCircle(float radius);
};
}
#endif