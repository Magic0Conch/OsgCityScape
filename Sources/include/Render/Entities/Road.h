#ifndef ROAD_H
#define ROAD_H
#include "BaseGeometry.h"
#include "osg/Math"
#include "osg/Quat"
#include "osg/Vec2f"
#include "osg/Vec3f"
#include "osg/ref_ptr"
#include <algorithm>
#include <corecrt_math_defines.h>
#include <memory>
#include <vector>
#define _USE_MATH_DEFINES 
#include "osg/Vec3"
#include<Windows.h>
#include <osg/Geometry>
#include <osg/Geode>
#include <cmath>

namespace CSEditor::Render {

class Road:public BaseGeometry{
private:
    float m_pathWidth;
    float m_innerRadius;
    std::unique_ptr<std::vector<osg::Vec3f>> m_pathKeyPoints;
    bool IsInferiorAngle(osg::Vec3f point0,osg::Vec3f point1,osg::Vec3f point2);
    bool isLeftSide(osg::Vec3f origin, osg::Vec3f target, osg::Vec3f checkPoint);
public:
    osg::Vec3f planeNormal;
    Road(std::unique_ptr<std::vector<osg::Vec3f>> pathKeyPoints,osg::Vec3f planeNormal = osg::Vec3f(0,1,0),float pathWidth=2.0f,float innerRadius = 0.0f,int segment=8);
    void setPathWidth(float rhs);

    void setInnerRadius(float rhs);

    void updateMesh() override;
};
}

#endif