#pragma once
#include "Core/ECS/Components/ProceduralMesh.h"
#include "GUI/Core/Panel.h"
namespace CSEditor::ECS {

class HighlightArea:public ProceduralMesh{
public:
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;    
    virtual void drawImGui() override;
    HighlightArea() = default;
    void setRadius(const char* propertyName,float radius);
    float getRadius(const char* propertyName);
    void setHeight(const char* propertyName,float height);
    float getHeight(const char* propertyName);
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


