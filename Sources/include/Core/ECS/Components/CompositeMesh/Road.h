#pragma once
#include "Core/ECS/Components/ProceduralMesh.h"
#include "GUI/Core/Panel.h"
#include "Render/Entities/Road.h"
namespace CSEditor::ECS {

class Road:public ProceduralMesh{
public:
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void drawImGui() override;
    Road() = default;
    // Road(osg::Vec3f planeNormal = osg::Vec3f(0,1,0),float pathWidth=2.0f,float innerRadius = 0.0f,int segment=8);

    void setPathWidth(const char* propertyName,float rhs);
    void setInnerRadius(const char* propertyName,float rhs);
    void setPlaneNormal(const char* propertyName,const osg::Vec3f& planeNormal);
    float getPathWidth(const char* propertyName) const;
    float getInnerRadius(const char* propertyName) const;
    osg::Vec3f getPlaneNormal(const char* propertyName) const;
// public:
// private:
//     osg::Vec3f m_planeNormal;
//     std::unique_ptr<std::vector<osg::Vec3f>> m_pathKeyPoints;
private:
    osg::ref_ptr<Render::Road> m_roadGeometry;
    float pathWidth = 1.0f;
    float innerRadius = 2.0f;

    float outerWidth = 0.198;
    osg::Vec4f backColor = osg::Vec4f(0.5377,0.5377,0.5377,0.5377);
    float flashFrequency = 1.0;
    osg::Vec4f outerColor = osg::Vec4f(0,0.968,1,0.877);
    float outerGradientLowerBound = 0;
    float patternDensity = 1.0;
    float patternWidth = 0.22;
    osg::Vec4f patternColor = osg::Vec4f(0.3162602,0.6164126,0.6509434,0.6470588);
    int patternShape = 2;
    float animSpeed = 1;
};
}


