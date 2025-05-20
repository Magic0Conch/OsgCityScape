#ifndef ROAD_H
#define ROAD_H
#include "BaseGeometry.h"
#include "osg/Vec3f"
#include <corecrt_math_defines.h>
#include <memory>
#include <vector>
#define _USE_MATH_DEFINES 
#include<Windows.h>
#include <osg/Geometry>
#include <osg/Geode>

namespace CSEditor::Render {

class Road:public BaseGeometry{
public:
    Road(std::unique_ptr<std::vector<osg::Vec3f>> pathKeyPoints,osg::Vec3f planeNormal = osg::Vec3f(0,1,0),float pathWidth=2.0f,float innerRadius = 1.0f,int segment=32);
    void setPathWidth(float rhs);
    void setInnerRadius(float rhs);
    float getPathWidth();
    float getInnerRadius();
    void update() override;

public:
    osg::Vec3f getPlaneNormal() const;
    void setPlaneNormal(const osg::Vec3f& planeNormal);
private:
    osg::Vec3f m_planeNormal;
    std::unique_ptr<std::vector<osg::Vec3f>> m_pathKeyPoints;
};
}

#endif