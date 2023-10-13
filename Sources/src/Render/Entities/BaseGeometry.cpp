#include <Windows.h>
#include "Render/Entities/BaseGeometry.h"

using namespace CSEditor::Render;
void BaseGeometry::UpdateBaseGeometryCallback::operator()(osg::Node* node,osg::NodeVisitor* nv){
    auto geom = dynamic_cast<BaseGeometry*>(node);
    geom->updateMesh();
    traverse(node, nv);
}
BaseGeometry::BaseGeometry(int segments):segments(segments){
    setUpdateCallback(new UpdateBaseGeometryCallback);
    indices = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);            
    addPrimitiveSet(indices);
}
void BaseGeometry::setSegments(float rhs){
    if(segments!=rhs){
        segments = rhs;
        isDirty = true;
    }
}

