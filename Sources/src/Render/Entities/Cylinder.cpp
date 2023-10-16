
#include "Render/Entities/Cylinder.h"


using namespace CSEditor::Render;

Cylinder::Cylinder(float radius,float height,int segment):m_radius(radius),m_height(height),BaseGeometry(segment){}

void Cylinder::setRadius(float rhs){
    if(m_radius!=rhs){
        m_radius = rhs;        
        m_isDirty = true;
    }
}

void Cylinder::setHeight(float rhs){
    if(m_height!=rhs){
        m_height = rhs;
        m_isDirty = true;
    }
}

void Cylinder::update(){ 
    if(m_isDirty){
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
        vertices->resizeArray((m_segments + 1) * 2);
        osg::ref_ptr<osg::Vec2Array> texCoords = new osg::Vec2Array();
        texCoords->resizeArray((m_segments + 1) * 2);
        float angleStep = 2.0f * M_PI / m_segments;
        for (int i = 0; i <= m_segments; i++)
        {
            float angle = i * angleStep;
            float x = cos(angle) * m_radius;
            float z = sin(angle) * m_radius;
            (*vertices)[i] = osg::Vec3(x, 0.f, z);
            float u = (float)i / m_segments;
            (*texCoords)[i] = osg::Vec2(u,0);
            (*vertices)[i + m_segments + 1]  = osg::Vec3(x,m_height, z);
            (*texCoords)[i + m_segments + 1] = osg::Vec2(u, m_height);
        }
        m_indices->resize(m_segments*6);
        for (int i = 0, ti = 0; i < m_segments; i++, ti += 6)
        {
            (*m_indices)[ti] = i;
            (*m_indices)[ti + 1] = i + m_segments + 1;
            (*m_indices)[ti + 2] = i + 1;
            (*m_indices)[ti + 3] = i + 1;
            (*m_indices)[ti + 4] = i + m_segments + 1;
            (*m_indices)[ti + 5] = i + m_segments + 2;

            if (i == m_segments - 1)
            {
                (*m_indices)[ti + 2] = 0;
                (*m_indices)[ti + 5] = m_segments + 1;
            }
        }
        setPrimitiveSet(0,m_indices);
        
        setVertexArray(vertices.get());
        setTexCoordArray(0,texCoords.get());
        setVertexAttribArray(0,getVertexArray());
        setVertexAttribArray(1,getTexCoordArray(0));
        m_isDirty = false;
    }
}
