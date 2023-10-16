#ifndef CIRCLE_H
#define CIRCLE_H
#include "BaseGeometry.h"
#include "osg/ref_ptr"
#include <corecrt_math_defines.h>
#include <memory>
#define _USE_MATH_DEFINES 
#include "osg/Vec3"
#include<Windows.h>
#include <osg/Geometry>
#include <osg/Geode>
#include <cmath>

namespace CSEditor::Render {

class Circle:public BaseGeometry{
public:
    Circle(float radius=1.0f,int segment=32);

    void setRadius(float rhs);

    void update() override;
private:
    float m_radius;
    
};
}

#endif