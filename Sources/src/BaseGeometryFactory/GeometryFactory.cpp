#include "BaseGeometryFactory/GeometryFactory.h"
using namespace cs;
osg::ref_ptr<osg::Geometry> BaseGeometryFactory::createBaseGeometry(const BaseGeometryType type){
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    switch (type) {
        case BaseGeometryType::QUAD:
            geom = osg::createTexturedQuadGeometry(osg::Vec3(-1,-1,0), osg::Vec3(2,0,0), osg::Vec3(0,2,0));
            geom->setVertexAttribArray(0,geom->getVertexArray());
            geom->setVertexAttribArray(1,geom->getTexCoordArray(0));
        break;

    }
    return geom;
}