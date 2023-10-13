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
    Cylinder(float radius=1.0f,float height = 2.0f,int segment=32);

    void setRadius(float rhs);

    void setHeight(float rhs);

    void updateMesh() override;
};
}

#endif