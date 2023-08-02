#ifndef GeometryFactory_H
#define GeometryFactory_H
#include <windows.h>
#include "osg/Geometry"
#include "osg/ref_ptr"
namespace cs{
enum class BaseGeometryType{
    QUAD
};

class BaseGeometryFactory{
public:
    static osg::ref_ptr<osg::Geometry> createBaseGeometry(const BaseGeometryType );
     
};
}
#endif