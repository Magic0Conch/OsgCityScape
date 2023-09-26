#ifndef BASEGEOMETRY_H
#define BASEGEOMETRY_H
#include <Windows.h>
#include "osg/Geometry"
namespace cs {
class BaseGeometry:public osg::Geometry{
private:
    virtual void updateMesh() = 0;
    class UpdateBaseGeometryCallback:public osg::NodeCallback{
    public:
        virtual void operator()(osg::Node* node,osg::NodeVisitor* nv){
            auto geom = dynamic_cast<BaseGeometry*>(node);
            geom->updateMesh();
            traverse(node, nv);
        }
    };
protected:
    bool isDirty = true;
    osg::ref_ptr<osg::DrawElementsUInt> indices;
    BaseGeometry(int segments):segments(segments){
        setUpdateCallback(new UpdateBaseGeometryCallback);
        indices = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);            
        addPrimitiveSet(indices);
    }
public:
    int segments;
    void setSegments(float rhs){
        if(segments!=rhs){
            segments = rhs;
            isDirty = true;
        }
    }

};
}

#endif