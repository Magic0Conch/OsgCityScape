#pragma once
#include "Core/ECS/Components/ProceduralMesh.h"
#include "GUI/Core/Panel.h"
namespace CSEditor::ECS {

class Road:public ProceduralMesh{
public:
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void drawImGui() override;
    Road() = default;
    Road(std::unique_ptr<std::vector<osg::Vec3f>> pathKeyPoints,osg::Vec3f planeNormal = osg::Vec3f(0,1,0),float pathWidth=2.0f,float innerRadius = 0.0f,int segment=8);
    // void setRadius(const char* propertyName,float radius);
    // float getRadius(const char* propertyName);
    // void setHeight(const char* propertyName,float height);
    // float getHeight(const char* propertyName);
    void setPathWidth(float rhs);
    void setInnerRadius(float rhs);

public:
    osg::Vec3f getPlaneNormal() const;
    void setPlaneNormal(const osg::Vec3f& planeNormal);
private:
    osg::Vec3f m_planeNormal;
    std::unique_ptr<std::vector<osg::Vec3f>> m_pathKeyPoints;



private:

    osg::Vec4 wallTintColor = osg::Vec4(0,0.87,1.,0.235);

    float wallTextureDensity = 1.9;
    float wallAnimSpeed = -.6;
    float wallPatternWidth = .258;
    osg::Vec4 wallPatternColor = osg::Vec4(0,0.87,1.,1.0);
    float wallFlashFrequency = 4;
    int wallBackStyle = 0;
    int wallPatternShape = 1;
    float wallOuterWidth = 0;

    float bottomOuterWidth = 0.2;
    osg::Vec4 bottomOuterTintColor = osg::Vec4(0,0.87,1.,1.0);
    osg::Vec4 bottomInnerTintColor = osg::Vec4(0,0.87,1.,0);
    bool wallFade = true;


    float bottomInnerAlphaScale = .547;

    float bottomFlashFrequency = 1.7;
    float bottomAnimSpeed = 0;
    float bottomPatternDensity = 1.73;
    float bottomPatternWidth = .039;
    osg::Vec4 bottomPatternColor = osg::Vec4(1.0,0.0,0.0,0.0);
    int bottomPatternShape = 1;
};
}


