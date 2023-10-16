#include <Windows.h>
#include "Render/Entities/BaseGeometry.h"

using namespace CSEditor::Render;
void BaseGeometry::UpdateBaseGeometryCallback::operator()(osg::Node* node,osg::NodeVisitor* nv){
    auto geom = dynamic_cast<BaseGeometry*>(node);
    geom->update();
    traverse(node, nv);
}
BaseGeometry::BaseGeometry(int segments):m_segments(segments){
    setUpdateCallback(new UpdateBaseGeometryCallback);
    m_indices = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);            
    addPrimitiveSet(m_indices);
}
void BaseGeometry::setSegments(float rhs){
    if(m_segments!=rhs){
        m_segments = rhs;
        m_isDirty = true;
    }
}

