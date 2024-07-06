#include "Core/ECS/Components/CompositeMesh/Road.h"
#include "Core/ECS/Components/Material.h"
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/Panel.h"
#include "Render/Entities/BaseGeometry.h"
#include "Render/Entities/Circle.h"
#include "Render/Entities/Cylinder.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "osg/BlendFunc"
#include "osg/Depth"
#include <osg/CullFace>
#include "Render/RenderSystem.h"
#include "osg/ref_ptr"
#include "Render/Entities/Road.h"
using namespace CSEditor::ECS;

void Road::loadResource(std::shared_ptr<Object> parentObject) {
    ProceduralMesh::loadResource(parentObject);
    auto pathKeyPoints = std::make_unique<std::vector<osg::Vec3f>>();
    pathKeyPoints->emplace_back(osg::Vec3f(-2.0,0.0,4.0));
    pathKeyPoints->emplace_back(osg::Vec3f(10.0,0.0,10.0));
    pathKeyPoints->emplace_back(osg::Vec3f(10.0,0.0,-10.0));
    pathKeyPoints->emplace_back(osg::Vec3f(20.0,0.0,-10.0));
    pathKeyPoints->emplace_back(osg::Vec3f(24.0,0.0,15.0));
    m_roadGeometry = new Render::Road(std::move(pathKeyPoints));
    
    const std::string& vertRoadPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.vert").string();
    const std::string& fragRoadPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/roadFX.frag").string();
    auto roadMat = std::make_shared<ECS::Material>(vertRoadPath, fragRoadPath);
    addGeometry(m_roadGeometry, roadMat);

    osg::ref_ptr<osg::BlendFunc> blend = new osg::BlendFunc;
    osg::ref_ptr<osg::Depth> depth = new osg::Depth;
    depth->setWriteMask(false);
    blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

    roadMat->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    roadMat->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
    roadMat->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
    roadMat->getStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
    roadMat->getStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
    roadMat->setVec4("_BackColor",backColor);
    roadMat->setFloat("_FlashFrequency",flashFrequency);
    roadMat->setFloat("_OuterWidth",outerWidth);
    roadMat->setFloat("_PatternDensity",patternDensity);
    roadMat->setFloat("_PatternWidth",patternWidth);
    roadMat->setVec4("_PatternColor",patternColor);
    roadMat->setInt("_PatternShape",patternShape);
    roadMat->setFloat("_AnimSpeed",animSpeed);
    roadMat->setFloat("_PathWidth",pathWidth);
    roadMat->setFloat("_OuterGradientLowerBound",outerGradientLowerBound);
    roadMat->setVec4("_OuterColor",outerColor);
    roadMat->setMatrix("_ProjectionMatrix",Core::g_runtimeContext.renderSystem->getMainProjectionMatrix());
}

// Road::Road(osg::Vec3f planeNormal,float pathWidth,float innerRadius,int segment):m_planeNormal(planeNormal){
//     setPathWidth(pathWidth);
//     setInnerRadius(innerRadius);
// }
// void HighlightArea::setRadius(const char* propertyName,float radius){
//     m_materials[1]->setFloat(propertyName,radius);
//     m_geometries[1]->setFloatProperty(propertyName, radius);
//     m_geometries[0]->setFloatProperty(propertyName, radius);
// }

// float HighlightArea::getRadius(const char* propertyName){
//     return m_geometries[0]->getProperty<float>(propertyName);
// }

// void HighlightArea::setHeight(const char* propertyName,float height){
//     m_materials[1]->setFloat(propertyName,height);
//     m_geometries[1]->setFloatProperty(propertyName, height);
// }

// float HighlightArea::getHeight(const char* propertyName){
//     return m_geometries[1]->getProperty<float>(propertyName);
// }

void Road::setPathWidth(const char* propertyName,float rhs){
    m_materials[0]->setFloat(propertyName,rhs);
    m_geometries[0]->setFloatProperty(propertyName, rhs);
}

void Road::setInnerRadius(const char* propertyName,float rhs){

}

void Road::setPlaneNormal(const char* propertyName,const osg::Vec3f& planeNormal){

}

float Road::getPathWidth(const char* propertyName) const{

}

float Road::getInnerRadius(const char* propertyName) const{

}

osg::Vec3f Road::getPlaneNormal(const char* propertyName) const{

}


void Road::drawImGui() {    
    if (ImGui::CollapsingHeader("Road FX")){
        if (ImGui::TreeNode("Geometry")){
            GUI::Panel::SliderIntWithSetter("Segments", *m_roadGeometry, m_roadGeometry->getProperty<int>("_Segments"), &Render::BaseGeometry::setSegments, 3,32);
            GUI::Panel::SliderFloatWithSetter("PathWidth","_PathWidth", *this, &Road::getPathWidth,&Road::setPathWidth, 0.1f,10.0f);
            GUI::Panel::SliderFloatWithSetter("InnerRadius", *m_roadGeometry, &Road::getInnerRadius,&Road::setInnerRadius, 0.1f,10.0f);            
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Bottom")){
            GUI::Panel::SliderFloatWithSetter("outerWidth","_OuterWidth", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,1.0f);
            GUI::Panel::EditColorWithSetter("backColor","_BackColor", *m_materials[0], &Material::getProperty<osg::Vec4>,&Material::setVec4);
            GUI::Panel::SliderFloatWithSetter("flashFrequency","_FlashFrequency", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,25);            
            GUI::Panel::EditColorWithSetter("outerColor","_OuterColor", *m_materials[0], &Material::getProperty<osg::Vec4>,&Material::setVec4);            
            GUI::Panel::SliderFloatWithSetter("outerGradientLowerBound","_OuterGradientLowerBound", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,1.0f);            
            GUI::Panel::SliderFloatWithSetter("patternDensity","_PatternDensity", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,5);            
            GUI::Panel::SliderFloatWithSetter("patternWidth","_PatternWidth", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,1);
            GUI::Panel::EditColorWithSetter("patternColor","_PatternColor", *m_materials[0], &Material::getProperty<osg::Vec4>,&Material::setVec4);
            GUI::Panel::SliderIntWithSetter("patternShape","_PatternShape", *m_materials[0], &Material::getProperty<int>,&Material::setInt, 0,2);
            GUI::Panel::SliderFloatWithSetter("animSpeed","_AnimSpeed", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,10.0f);
            ImGui::TreePop();
        }
    }
}