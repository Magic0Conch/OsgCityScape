#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include <windows.h>
#include "BaseGeometryFactory/Road.h"
#include "Render/Material.h"
#include "Render/RenderPipelinePostProcess.h"
#include "UI/ImGuiInitOperation.h"
#include "Utils/ShaderUtils.h"
#include "imgui.h"
#include "osg/Camera"
#include "osg/Depth"
#include "osg/BlendFunc"
#include "osg/GLExtensions"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/StateAttribute"
#include "osg/Transform"
#include "osg/Vec3f"
#include "osg/Vec4"
#include "osg/Vec4f"
#include "osg/ref_ptr"
#include <osgGA/FlightManipulator>
#include <iostream>
#include <osgViewer/Viewer>
#include "UI/OsgImGuiHandler.h"
#include<Windows.h>
#include <stdio.h>

const auto resolution = osg::Vec2i(1728,702);

int segments=21;
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

osg::ref_ptr<cs::Road> roadGeometry;
osg::Camera* cam;
namespace cs {
    class ImGuiDemo : public OsgImGuiHandler
    {
    protected:
        virtual void drawUi() override{
            const auto main_viewport = ImGui::GetMainViewport();
            ImGuiWindowFlags window_flags = 0;
            ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);

            bool p_open = true;
            if(ImGui::Begin("Area Highlight",&p_open,window_flags)){
                if (ImGui::TreeNode("Geometry")){
                    ImGui::SliderInt("Segments",&segments,2,32);
                    if(ImGui::IsItemEdited()){
                        roadGeometry->setSegments(segments);
                    }
                    ImGui::DragFloat("PathWidth",&pathWidth);
                    if(ImGui::IsItemEdited()){
                        roadGeometry->setPathWidth(pathWidth);
                    }
                    ImGui::DragFloat("InnerRadius",&innerRadius);
                    if(ImGui::IsItemEdited()){
                        roadGeometry->setInnerRadius(innerRadius);
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Pattern")){
                    ImGui::SliderFloat("outerWidth",&outerWidth,0,1);
                    ImGui::ColorEdit4("backColor",reinterpret_cast<float*>(&backColor));
                    ImGui::SliderFloat("flashFrequency",&flashFrequency,0,25);
                    ImGui::ColorEdit4("outerColor",reinterpret_cast<float*>(&outerColor));
                    ImGui::SliderFloat("outerGradientLowerBound",&outerGradientLowerBound,0,1);
                    ImGui::SliderFloat("patternDensity",&patternDensity,0,5);
                    ImGui::SliderFloat("patternWidth",&patternWidth,0,1);
                    ImGui::ColorEdit4("patternColor",reinterpret_cast<float*>(&patternColor));
                    ImGui::SliderInt("patternShape",&patternShape,0,2);
                    ImGui::SliderFloat("animSpeed",&animSpeed,0,10);

                    // ImGui::SliderFloat("_Outline",&bottomOuterWidth, 0.0f, 1.0f);
                    // ImGui::ColorEdit4("_InnerColor",reinterpret_cast<float*>(&bottomInnerTintColor));
                    ImGui::TreePop();
                }
            }
            
            ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
            ImGui::PopItemWidth();
            ImGui::End();
        }
    };

    std::shared_ptr<RenderPipelinePostProcess> roadFXPipeline;
    

    auto setupScene(){
        roadFXPipeline.reset(new RenderPipelinePostProcess());
        auto pathKeyPoints = std::make_unique<std::vector<osg::Vec3f>>();
        pathKeyPoints->emplace_back(osg::Vec3f(-2.0,0.0,4.0));
        pathKeyPoints->emplace_back(osg::Vec3f(10.0,0.0,10.0));
        pathKeyPoints->emplace_back(osg::Vec3f(10.0,0.0,-10.0));
        pathKeyPoints->emplace_back(osg::Vec3f(20.0,0.0,-10.0));
        pathKeyPoints->emplace_back(osg::Vec3f(24.0,0.0,15.0));
         
        roadGeometry = new Road(std::move(pathKeyPoints));
        roadGeometry->setPathWidth(pathWidth);
        roadGeometry->setInnerRadius(innerRadius);
        roadGeometry->setSegments(segments);
        osg::ref_ptr<osg::Geode> roadGeode = new osg::Geode;
        roadGeode->addDrawable(roadGeometry);
        
        auto materialRoadFX= std::make_unique<Material>(roadGeode,"resources/shaders/triangle.vert", "resources/shaders/roadFX.frag");


        materialRoadFX->addUniform("_BackColor",&backColor);
        materialRoadFX->addUniform("_FlashFrequency",&flashFrequency);
        materialRoadFX->addUniform("_OuterWidth",&outerWidth);
        materialRoadFX->addUniform("_PatternDensity",&patternDensity);
        materialRoadFX->addUniform("_PatternWidth",&patternWidth);
        materialRoadFX->addUniform("_PatternColor",&patternColor);
        materialRoadFX->addUniform("_PatternShape",&patternShape);
        materialRoadFX->addUniform("_AnimSpeed",&animSpeed);
        materialRoadFX->addUniform("_PathWidth",&pathWidth);
        materialRoadFX->addUniform("_OuterGradientLowerBound",&outerGradientLowerBound);
        materialRoadFX->addUniform("_OuterColor",&outerColor);
        roadGeode->setUpdateCallback(new UpdateUniformCallback(materialRoadFX.get()));
        
        // osg::ref_ptr<osg::BlendFunc> blend = new osg::BlendFunc;
        // osg::ref_ptr<osg::Depth> depth = new osg::Depth;
        // depth->setWriteMask(false);
        // depth->setRange(0.0f, 0.0f);
        // blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

        // roadGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        // roadGeode->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
        // roadGeode->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
        // roadGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
        
        osg::ref_ptr<osg::Group> scene = new osg::Group;
        
        //display quad
        scene->addChild(roadGeode);
        //off-screen rendering
        // roadFXPipeline->addRenderPassesToOsgGroup(*scene);
        return scene;
    }

}

int main(){
    using namespace cs;
    osg::DisplaySettings::instance()->setGLContextVersion("3.3");
    osg::DisplaySettings::instance()->setGLContextProfileMask(0x1); 
    std::cout<< osg::DisplaySettings::instance()->getGLContextVersion();
    osgViewer::Viewer viewer;
    osg::ref_ptr<osgGA::FirstPersonManipulator> manipulator = new osgGA::FirstPersonManipulator();
    manipulator->setHomePosition(osg::Vec3d(0,90,0), osg::Vec3d(0,0,0),osg::Vec3f(0,0,1));
    viewer.setCameraManipulator(manipulator);
    viewer.setUpViewInWindow(100, 100,resolution.x(),resolution.y());
    viewer.addEventHandler(new osgViewer::StatsHandler());
    viewer.addEventHandler(new ImGuiDemo);

    viewer.setRealizeOperation(new ImGuiInitOperation);


    auto scene = setupScene();
    viewer.setSceneData(scene.get());
    viewer.realize();
    while (!viewer.done())
    {
        viewer.frame();
    }
    return 0;                
}

    // if(ImGui::Button("set position")){
    //     auto camViewMat = cam->getViewMatrix();
    //     auto camPos = camViewMat.getTrans();
    //     std::cout<<camPos.x()<<","<<camPos.y()<<","<<camPos.z()<<std::endl;                        
    //     auto q = camViewMat.getRotate();
    //     std::cout<<q.x()<<","<<q.y()<<","<<q.z()<<q.w()<<std::endl;

    // }
    // cam = viewer.getCamera();
    // cam->setReferenceFrame(osg::Transform::ABSOLUTE_RF);