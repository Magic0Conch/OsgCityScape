#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include <windows.h>
#include "Render/Entities/Road.h"
#include "Render/Effects/Bloom.h"
#include "GUI/Core/UIManager.h"
#include "Render/LowRender/RTTCamera.h"
#include "imgui.h"
#include "osg/Camera"
#include "osg/Depth"
#include "osg/BlendFunc"
#include "osg/GLExtensions"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/Multisample"
#include "osg/StateAttribute"
#include "osg/Transform"
#include "osg/Vec3f"
#include "osg/Vec4"
#include "osg/Vec4f"
#include "osg/ref_ptr"
#include <osgGA/FlightManipulator>
#include <iostream>
#include <osgViewer/Viewer>
#include "GUI/Helper/ImGuiInitOperation.h"
#include <Windows.h>
#include <stdio.h>
#include <nfd.h>

using namespace CSEditor::Render;
using namespace CSEditor::GUI;

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

//bloom parameters
int blurIterations = 4;
float blurSpeed = 0.6f;
int downSample = 2;
float luminanceThreshold = 0.4f;
osg::ref_ptr<Road> roadGeometry;
osg::Camera* cam;
class PanelProperties : public UIManager
{
protected:
    virtual void drawUi() override{
        const auto main_viewport = ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags = 0;
        ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);

        bool p_open = true;
        if(ImGui::Begin("Path FX",&p_open,window_flags)){
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
            if (ImGui::TreeNode("Bloom")){
                ImGui::SliderFloat("blurSpeed", &blurSpeed,0.2f,10.0f);
                ImGui::SliderFloat("_LuminanceThreshold", &luminanceThreshold,0,1);
                // ImGui::SliderInt("blurIterations", &blurIterations,0,32);
                ImGui::TreePop();
            }
        }
        
        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
        ImGui::PopItemWidth();
        ImGui::End();
    }
};

std::shared_ptr<RenderPipelinePostProcess> roadFXPipeline;
osg::ref_ptr<Bloom> bloomPipeline;

class BloomUniformCallback:public UpdateUniformCallback{
public:
    BloomUniformCallback(std::shared_ptr<RenderPipelinePostProcess> rp):UpdateUniformCallback(rp){};
    virtual void setUniforms() override{
        const auto& renderPasses = bloomPipeline->getGaussianPipeline()->getRenderPasses();
        for (int i = 0; i<blurIterations; i++) {
            renderPasses[i*2+1]->getMaterial()->setUniform("_BlurSize", 1.f+i*blurSpeed);
            renderPasses[i*2+2]->getMaterial()->setUniform("_BlurSize", 1.f+i*blurSpeed);
        }
    }
};


osg::ref_ptr< osg::GraphicsContext > setupGC(){
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
    traits->x = 20; traits->y = 30;
    traits->width = resolution.x(); traits->height = resolution.y();
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->glContextProfileMask = 0x1;// 0x1;// 
    osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
    if( !gc.valid() )
    {
        osg::notify( osg::FATAL ) << "Unable to create OpenGL v" << " context." << std::endl;
        return nullptr;
    }
    gc->getState()->resetVertexAttributeAlias(false);
    gc->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);
    return gc;
}

auto setupScene(){
    roadFXPipeline.reset(new RenderPipelinePostProcess());
    //set roadGeometry init parameters.
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

    //Bind shader file and set uniform parameters
    auto materialRoadFX= std::make_unique<Material>("resources/shaders/triangle.vert", "resources/shaders/roadFX.frag");
    materialRoadFX->bind(roadGeode);
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
    
    //Adjust render state(depth write off,blend on)
    osg::ref_ptr<osg::BlendFunc> blend = new osg::BlendFunc;
    osg::ref_ptr<osg::Depth> depth = new osg::Depth;

    depth->setWriteMask(false);
    depth->setRange(0.0f, 0.0f);
    blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

    roadGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    roadGeode->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
    roadGeode->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
    roadGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
    roadGeode->getStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);

    //post process
    //render geode to a screen texture
    osg::ref_ptr<RenderTexture> screenTexture = new RenderTexture(resolution.x(),resolution.y());
    osg::ref_ptr<RTTCamera> screenRenderPass = new RTTCamera(screenTexture);
    
    screenRenderPass->addChildToRTTCamera(roadGeode);
    screenRenderPass->setMutisample(true);
    
    roadFXPipeline->addRenderPass(screenRenderPass);
    //bloom:render screen texture
    bloomPipeline = new Bloom(screenTexture,nullptr,blurIterations,blurSpeed,&luminanceThreshold);
    roadFXPipeline->addRenderPipeline(bloomPipeline);
    roadFXPipeline->getDestinationQuadGeode()->setUpdateCallback(new BloomUniformCallback(roadFXPipeline));
    // bloomPipeline->enableUpdateUniformPerFrame();
    osg::ref_ptr<osg::Group> scene = new osg::Group;
    // scene->addChild(roadGeode);
    
    //display quad
    scene->addChild(roadFXPipeline->getDestinationQuadGeode());
    
    //off-screen rendering
    roadFXPipeline->addRenderPassesToOsgGroup(*scene);
    return scene;
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

int main( int argc, char** argv )
{
    using namespace CSEditor;
    
    osgViewer::Viewer viewer;
    viewer.addEventHandler(new osgViewer::StatsHandler());
    osg::ref_ptr<osgGA::FirstPersonManipulator> manipulator = new osgGA::FirstPersonManipulator();
    manipulator->setHomePosition(osg::Vec3d(0,40,0), osg::Vec3d(0,0,0),osg::Vec3f(0,0,1));
    viewer.setCameraManipulator(manipulator);
    viewer.setSceneData(setupScene());
    viewer.addEventHandler(new PanelProperties);
    viewer.setRealizeOperation(new ImGuiInitOperation);
    osg::Camera* cam = viewer.getCamera();

    auto gc = setupGC();
    cam->setGraphicsContext( gc.get() );
    cam->setProjectionMatrix( osg::Matrix::perspective( 30., (double)resolution.x()/(double)resolution.y(), 1., 100. ) );
    cam->setViewport( new osg::Viewport( 0, 0, resolution.x(), resolution.y() ) );

    return( viewer.run() );
}
