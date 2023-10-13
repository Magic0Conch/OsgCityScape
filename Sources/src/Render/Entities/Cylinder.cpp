
#include "Render/Entities/Cylinder.h"


using namespace CSEditor::Render;

Cylinder::Cylinder(float radius,float height,int segment):radius(radius),height(height),BaseGeometry(segment){}

void Cylinder::setRadius(float rhs){
    if(radius!=rhs){
        radius = rhs;        
        isDirty = true;
    }
}

void Cylinder::setHeight(float rhs){
    if(height!=rhs){
        height = rhs;
        isDirty = true;
    }
}

void Cylinder::updateMesh(){ 
    if(isDirty){
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
        vertices->resizeArray((segments + 1) * 2);
        osg::ref_ptr<osg::Vec2Array> texCoords = new osg::Vec2Array();
        texCoords->resizeArray((segments + 1) * 2);
        float angleStep = 2.0f * M_PI / segments;
        for (int i = 0; i <= segments; i++)
        {
            float angle = i * angleStep;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;
            (*vertices)[i] = osg::Vec3(x, 0.f, z);
            float u = (float)i / segments;
            (*texCoords)[i] = osg::Vec2(u,0);
            (*vertices)[i + segments + 1]  = osg::Vec3(x, height, z);
            (*texCoords)[i + segments + 1] = osg::Vec2(u, height);
        }
        indices->resize(segments*6);
        for (int i = 0, ti = 0; i < segments; i++, ti += 6)
        {
            (*indices)[ti] = i;
            (*indices)[ti + 1] = i + segments + 1;
            (*indices)[ti + 2] = i + 1;
            (*indices)[ti + 3] = i + 1;
            (*indices)[ti + 4] = i + segments + 1;
            (*indices)[ti + 5] = i + segments + 2;

            if (i == segments - 1)
            {
                (*indices)[ti + 2] = 0;
                (*indices)[ti + 5] = segments + 1;
            }
        }
        setPrimitiveSet(0,indices);
        
        setVertexArray(vertices.get());
        setTexCoordArray(0,texCoords.get());
        setVertexAttribArray(0,getVertexArray());
        setVertexAttribArray(1,getTexCoordArray(0));
        isDirty = false;
    }
}
