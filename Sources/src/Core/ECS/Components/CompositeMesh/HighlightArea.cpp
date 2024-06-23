#include "Core/ECS/Components/CompositeMesh/HighlightArea.h"
#include "Core/ECS/Components/Material.h"
#include "Core/ECS/Components/ProceduralMesh.h"
#include "GUI/Core/Panel.h"
#include "Render/RenderSystem.h"
#include "Editor/Core/RuntimeContext.h"
#include "Render/Entities/Circle.h"
#include "Render/Entities/Cylinder.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "osg/BlendFunc"
#include "osg/Depth"
#include <osg/CullFace>

using namespace CSEditor::ECS;

void HighlightArea::loadResource(std::shared_ptr<Object> parentObject) {
    ProceduralMesh::loadResource(parentObject);
    osg::ref_ptr<Render::BaseGeometry> circle = new Render::Circle(1.0f);
    const std::string& vertCirclePath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCircle.vert").string();
    const std::string& fragCirclePath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCircle.frag").string();
    auto circleMat = std::make_shared<ECS::Material>(vertCirclePath, fragCirclePath);
    addGeometry(circle, circleMat);

    osg::ref_ptr<Render::BaseGeometry> geometryCylinder = new Render::Cylinder();
    const std::string& vertCylinderPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCylinder.vert").string();
    const std::string& fragCylinderPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCylinder.frag").string();
    std::shared_ptr<ECS::Material> cylinderMat = std::make_shared<ECS::Material>(vertCylinderPath,fragCylinderPath);
    addGeometry(geometryCylinder, cylinderMat);
    
    osg::ref_ptr<osg::BlendFunc> blend = new osg::BlendFunc;
    osg::ref_ptr<osg::Depth> depth = new osg::Depth;
    depth->setWriteMask(false);
    blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

    circleMat->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    circleMat->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
    circleMat->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
    circleMat->getStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
    circleMat->getStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
    circleMat->setFloat("_OutlineRatio",0.2);
    circleMat->setVec4("_InnerColor",osg::Vec4(0,0.87,1.,0));
    circleMat->setFloat("_FlashFrequency",1.7);
    circleMat->setFloat("_PatternDensity",1.73);
    circleMat->setFloat("_PatternWidth",.039);
    circleMat->setVec4("_PatternColor",osg::Vec4(1.0,0.0,0.0,0.0));
    circleMat->setInt("_PatternShape",1);
    circleMat->setFloat("_AnimSpeed",0);
    circleMat->setVec4("_OutlineColor",osg::Vec4(0,0.87,1.,1.0));
    circleMat->setMatrix("_ProjectionMatrix",Core::g_runtimeContext.renderSystem->getMainProjectionMatrix());
    

    cylinderMat->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    cylinderMat->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
    cylinderMat->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
    cylinderMat->getStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
    cylinderMat->getStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
    cylinderMat->setVec4("_Color",wallTintColor);
    cylinderMat->setVec4("_PatternColor",wallPatternColor);
    cylinderMat->setFloat("_AnimSpeed",wallAnimSpeed);
    cylinderMat->setFloat("_PatternDensity",wallTextureDensity);
    cylinderMat->setFloat("_PatternWidth",wallPatternWidth);
    cylinderMat->setFloat("_FlashFrequency",wallFlashFrequency);
    cylinderMat->setFloat("_OuterWidth",wallOuterWidth);
    cylinderMat->setInt("_PatternShape",wallPatternShape);
    cylinderMat->setInt("_BackStyle",wallBackStyle);
    cylinderMat->setBool("_Fade",wallFade);
    cylinderMat->setFloat("_Radius",1);
    cylinderMat->setFloat("_Height",2);
    cylinderMat->setMatrix("_ProjectionMatrix",Core::g_runtimeContext.renderSystem->getMainProjectionMatrix());    
}

void HighlightArea::setRadius(const char* propertyName,float radius){
    m_materials[1]->setFloat(propertyName,radius);
    m_geometries[1]->setFloatProperty(propertyName, radius);
    m_geometries[0]->setFloatProperty(propertyName, radius);
}

float HighlightArea::getRadius(const char* propertyName){
    return m_geometries[0]->getProperty<float>(propertyName);
}

void HighlightArea::setHeight(const char* propertyName,float height){
    m_materials[1]->setFloat(propertyName,height);
    m_geometries[1]->setFloatProperty(propertyName, height);
}

float HighlightArea::getHeight(const char* propertyName){
    return m_geometries[1]->getProperty<float>(propertyName);
}

void HighlightArea::drawImGui() {    
    if (ImGui::CollapsingHeader("HighLight Area")){
        if (ImGui::TreeNode("Geometry")){
            GUI::Panel::SliderFloatWithSetter("Radius","_Radius", *this, &HighlightArea::getRadius,&HighlightArea::setRadius, 0.1f,10.0f);
            GUI::Panel::SliderFloatWithSetter("Height","_Height", *this, &HighlightArea::getHeight,&HighlightArea::setHeight, 0.1f,10.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Bottom")){
            GUI::Panel::EditColorWithSetter("Outer Tint Color","_OutlineColor", *m_materials[0], &Material::getProperty<osg::Vec4>,&Material::setVec4);        
            GUI::Panel::EditColorWithSetter("Inner Tint Color","_InnerColor", *m_materials[0], &Material::getProperty<osg::Vec4>,&Material::setVec4);
            GUI::Panel::EditColorWithSetter("Pattern Color","_PatternColor", *m_materials[0], &Material::getProperty<osg::Vec4>,&Material::setVec4);
            GUI::Panel::SliderIntWithSetter("Pattern Shape","_PatternShape", *m_materials[0], &Material::getProperty<int>,&Material::setInt, 0,7);
            GUI::Panel::SliderFloatWithSetter("Flash Frequency","_FlashFrequency", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,10.0f);
            GUI::Panel::SliderFloatWithSetter("Animation Speed","_AnimSpeed", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, -1.0f,1.0f);
            GUI::Panel::SliderFloatWithSetter("Pattern Density","_PatternDensity", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,10.0f);
            GUI::Panel::SliderFloatWithSetter("Pattern Width","_PatternWidth", *m_materials[0], &Material::getProperty<float>,&Material::setFloat, 0.0f,1.0f);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Wall")){
            GUI::Panel::EditColorWithSetter("Tint Color","_Color", *m_materials[1], &Material::getProperty<osg::Vec4>,&Material::setVec4);
            GUI::Panel::SliderFloatWithSetter("Texture Density","_PatternDensity", *m_materials[1], &Material::getProperty<float>,&Material::setFloat, 0.0f,10.0f);        
            GUI::Panel::SliderFloatWithSetter("Animation Speed","_AnimSpeed", *m_materials[1], &Material::getProperty<float>,&Material::setFloat, -1.0f,1.0f);
            GUI::Panel::SliderFloatWithSetter("Pattern Width","_PatternWidth", *m_materials[1], &Material::getProperty<float>,&Material::setFloat, 0.0f,1.0f);
            GUI::Panel::EditColorWithSetter("Pattern Color","_PatternColor", *m_materials[1], &Material::getProperty<osg::Vec4>,&Material::setVec4);        
            GUI::Panel::SliderFloatWithSetter("Flash Frequency","_FlashFrequency", *m_materials[1], &Material::getProperty<float>,&Material::setFloat, 0.0f,10.0f);        
            GUI::Panel::SliderIntWithSetter("Back Style","_BackStyle", *m_materials[1], &Material::getProperty<int>,&Material::setInt, 0,1);
            GUI::Panel::SliderIntWithSetter("Pattern Shape","_PatternShape", *m_materials[1], &Material::getProperty<int>,&Material::setInt, 0,3);
            GUI::Panel::SliderFloatWithSetter("Outer Width","_OuterWidth", *m_materials[1], &Material::getProperty<float>,&Material::setFloat, 0.0f,1.0f);
            GUI::Panel::checkBoxWithSetter("Fade","_Fade", *m_materials[1], &Material::getProperty<bool>,&Material::setBool);
            ImGui::TreePop();
        }            
    }
}