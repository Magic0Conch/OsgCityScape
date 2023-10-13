#ifndef CYLINDER_H
#define CYLINDER_H

#include "BaseGeometry.h"
#include <corecrt_math_defines.h>
#define _USE_MATH_DEFINES 
#include "osg/Vec3"
#include<Windows.h>
#include <osg/Geometry>
#include <osg/Geode>
#include <cmath>

namespace CSEditor::Render {
class Cylinder:public BaseGeometry{
public:
    float radius;
    float height;    
public:
    Cylinder(float radius=1.0f,float height = 2.0f,int segment=32):radius(radius),height(height),BaseGeometry(segment){}

    void setRadius(float rhs){
        if(radius!=rhs){
            radius = rhs;        
            isDirty = true;
        }
    }

    void setHeight(float rhs){
        if(height!=rhs){
            height = rhs;
            isDirty = true;
        }
    }

    void updateMesh() override{ 
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
};
}

#endif