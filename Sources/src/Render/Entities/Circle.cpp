#include "Render/Entities/Circle.h"

using namespace CSEditor::Render;


Circle::Circle(float radius,int segment):radius(radius),BaseGeometry(segment){}

void Circle::setRadius(float rhs){
    if(rhs!=radius){
        radius = rhs;       
        isDirty = true; 
    }
}

void Circle::updateMesh(){
    if(isDirty){
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
        vertices->resizeArray(segments+1);
        osg::ref_ptr<osg::Vec2Array> texCoords = new osg::Vec2Array();
        texCoords->resizeArray(segments+1);
        osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
        indices->resize(segments*3);
        osg::Vec3 center = osg::Vec3(0,0,0);
        (*vertices)[0] = center;
        float angle = 0.0f;
        float angleStep = 2.0f * M_PI / segments;

        for (int i = 1; i<=segments; i++) {
            float x = sin(angle) * radius + center.x();
            float y = center.y();
            float z = cos(angle) * radius + center.z();

            (*vertices)[i] = osg::Vec3(x, y, z);
            (*texCoords)[i] = osg::Vec2(1,1);
            if (i < segments)
            {
                (*indices)[(i - 1) * 3] = 0;
                (*indices)[(i - 1) * 3 + 1] = i;
                (*indices)[(i - 1) * 3 + 2] = i + 1;
            }
            else
            {
                (*indices)[(i - 1) * 3] = 0;
                (*indices)[(i - 1) * 3 + 1] = i;
                (*indices)[(i - 1) * 3 + 2] = 1;
            }
            angle += angleStep;
        }
        setVertexArray(vertices.get());
        setTexCoordArray(0,texCoords.get());
        setVertexAttribArray(0,getVertexArray());
        setVertexAttribArray(1,getTexCoordArray(0));
        setPrimitiveSet(0,indices);
        isDirty = false;
    }
}
