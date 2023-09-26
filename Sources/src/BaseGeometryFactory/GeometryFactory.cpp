#include "BaseGeometryFactory/GeometryFactory.h"
#include "BaseGeometryFactory/Circle.h"
#include "osg/Array"
#include "osg/Shape"
#include "osg/ref_ptr"
using namespace cs;


osg::ref_ptr<osg::Geometry> BaseGeometryFactory::createBaseGeometry(const BaseGeometryType type){
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    switch (type) {
        case BaseGeometryType::QUAD:{
            geom = osg::createTexturedQuadGeometry(osg::Vec3(-1,-1,0), osg::Vec3(2,0,0), osg::Vec3(0,2,0));
            geom->setVertexAttribArray(0,geom->getVertexArray());
            geom->setVertexAttribArray(1,geom->getTexCoordArray(0));
            break;
        }
        // case BaseGeometryType::Circle:{
        //     return new Circle();
        //     break;
        // }
        // case BaseGeometryType::Cylinder:{
        //     osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        //     break;
        // }
        default:{
            //do nothing
        }
    }
    return geom;
}