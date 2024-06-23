#include <Windows.h>
#include <any>
#include "Render/Entities/BaseGeometry.h"
#include "osg/GLExtensions"

using namespace CSEditor::Render;
void BaseGeometry::UpdateBaseGeometryCallback::operator()(osg::Node* node,osg::NodeVisitor* nv){
    auto geom = dynamic_cast<BaseGeometry*>(node);
    geom->update();
    traverse(node, nv);
}
BaseGeometry::BaseGeometry(int segments){
    setIntProperty("_Segments",segments);
    setUpdateCallback(new UpdateBaseGeometryCallback);
    m_indices = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);            
    addPrimitiveSet(m_indices);
}

void BaseGeometry::setFloatProperty(const std::string& key,float value){
    if(m_properties.find(key)==m_properties.end()||std::any_cast<float>(m_properties[key])!=value){
        m_properties[key] = value;
        m_isDirty = true;
    }    
}

void BaseGeometry::setIntProperty(const std::string& key,int value){
    if(m_properties.find(key)==m_properties.end()||std::any_cast<int>(m_properties[key])!=value){
        m_properties[key] = value;
        m_isDirty = true;
    }    
}

void BaseGeometry::setSegments(float rhs){
    setIntProperty("_Segments", rhs);
}

std::unordered_map<std::string, std::any>& BaseGeometry::getProperties(){
    return m_properties;
}
// void BaseGeometry::drawImplementation(osg::RenderInfo& renderInfo) const{
//     auto ext = renderInfo.getState()->get<osg::GLExtensions>();
    
//     if (_containsDeprecatedData)
//     {
//         OSG_WARN<<"Geometry::drawImplementation() unable to render due to deprecated data, call geometry->fixDeprecatedData();"<<std::endl;
//         return;
//     }

//     osg::State& state = *renderInfo.getState();

//     // bool usingVertexBufferObjects = state.useVertexBufferObject(_supportsVertexBufferObjects && _useVertexBufferObjects);
//     // bool usingVertexArrayObjects = usingVertexBufferObjects && state.useVertexArrayObject(_useVertexArrayObject);

//     osg::VertexArrayState* vas = state.getCurrentVertexArrayState();
//     vas->setVertexBufferObjectSupported(true);

//     bool checkForGLErrors = state.getCheckForGLErrors()==osg::State::ONCE_PER_ATTRIBUTE;
//     if (checkForGLErrors) state.checkGLErrors("start of Geometry::drawImplementation()");


    

//     osg::AttributeDispatchers& attributeDispatchers = state.getAttributeDispatchers();

//     attributeDispatchers.reset();
//     attributeDispatchers.setUseVertexAttribAlias(true);
//     for(unsigned int unit=0;unit<_vertexAttribList.size();++unit)
//     {
//         attributeDispatchers.activateVertexAttribArray(unit, _vertexAttribList[unit].get());
//     }

//     // activate or dispatch any attributes that are bound overall
//     attributeDispatchers.activateNormalArray(_normalArray.get());
//     attributeDispatchers.activateColorArray(_colorArray.get());
//     attributeDispatchers.activateSecondaryColorArray(_secondaryColorArray.get());
//     attributeDispatchers.activateFogCoordArray(_fogCoordArray.get());

//     if (state.useVertexArrayObject(_useVertexArrayObject))
//     {
//         if (!vas->getRequiresSetArrays()) return;
//     }

//     vas->lazyDisablingOfVertexAttributes();

//     // set up arrays
//     if( _vertexArray.valid() )
//         vas->setVertexArray(state, _vertexArray.get());

//     if (_normalArray.valid() && _normalArray->getBinding()==osg::Array::BIND_PER_VERTEX)
//         vas->setNormalArray(state, _normalArray.get());

//     if (_colorArray.valid() && _colorArray->getBinding()==osg::Array::BIND_PER_VERTEX)
//         vas->setColorArray(state, _colorArray.get());

//     if (_secondaryColorArray.valid() && _secondaryColorArray->getBinding()==osg::Array::BIND_PER_VERTEX)
//         vas->setSecondaryColorArray(state, _secondaryColorArray.get());

//     if (_fogCoordArray.valid() && _fogCoordArray->getBinding()==osg::Array::BIND_PER_VERTEX)
//         vas->setFogCoordArray(state, _fogCoordArray.get());

//     for(unsigned int unit=0;unit<_texCoordList.size();++unit)
//     {
//         const osg::Array* array = _texCoordList[unit].get();
//         if (array)
//         {
//             vas->setTexCoordArray(state, unit,array);
//         }
//     }


//     for(unsigned int index = 0; index < _vertexAttribList.size(); ++index)
//     {
//         const osg::Array* array = _vertexAttribList[index].get();
//         if (array && array->getBinding()==osg::Array::BIND_PER_VERTEX)
//         {
//             vas->setVertexAttribArray(state, index, array);
//         }
//     }

//     vas->applyDisablingOfVertexAttributes(state);

//     if (checkForGLErrors) state.checkGLErrors("Geometry::drawImplementation() after vertex arrays setup.");


//     drawPrimitivesImplementation(renderInfo);

//     // unbind the VBO's if any are used.
//     vas->unbindVertexBufferObject();
//     vas->unbindElementBufferObject();

//     if (checkForGLErrors) state.checkGLErrors("end of Geometry::drawImplementation().");
// }   