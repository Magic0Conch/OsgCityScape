#include <memory>
#include <windows.h>
#include "BaseGeometryFactory/Circle.h"
#include "BaseGeometryFactory/Cylinder.h"
#include "BaseGeometryFactory/GeometryFactory.h"
#include "Render/Effects/GaussianBlur.h"
#include "Render/Material.h"
#include "Render/RenderPipelinePostProcess.h"
#include "Render/RTTCamera.h"
#include "UI/ImGuiInitOperation.h"
#include "Utils/ShaderUtils.h"
#include "imgui.h"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/StateAttribute"
#include "osg/Vec2i"
#include "osg/Vec4"
#include "osg/ref_ptr"
#include "osg/Depth"
#include <iostream>
#include <osgViewer/Viewer>
#include "UI/OsgImGuiHandler.h"
#include <osg/BlendFunc>
#include <osg/CullFace>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/View>
const auto resolution = osg::Vec2i(1728,702);

float radius=1.0f;

float height=2.0f;
osg::Vec4 wallTintColor = osg::Vec4(0,0.87,1.,1.0);

float wallTextureDensity = 1.9;
float wallAnimSpeed = -.6;
float wallPatternWidth = .258;
osg::Vec4 wallPatternColor = osg::Vec4(0,0.87,1.,1.0);
float wallFlashFrequency = 0;
int wallBackStyle = 0;
int wallPatternShape = 0;
float wallOuterWidth = 0;

float bottomOuterWidth = 0.103;
osg::Vec4 bottomOuterTintColor = osg::Vec4(0,0.87,1.,1.0);;
osg::Vec4 bottomInnerTintColor = osg::Vec4(0,0.87,1.,1.0);;
bool wallFade = true;


float bottomInnerAlphaScale = .547;

float bottomFlashFrequency = 1.7;
float bottomAnimSpeed = 0;
float bottomPatternDensity = 1.73;
float bottomPatternWidth = .039;
osg::Vec4 bottomPatternColor = osg::Vec4(1.0,0.0,0.0,0.0);
int bottomPatternShape = 1;

//bloom
int blurIterations = 4;
float blurSpeed = 0.6f;
int downSample = 2;
float luminanceThreshold = 0.4f;

namespace cs {
    osg::ref_ptr<Circle> circle;
    osg::ref_ptr<Cylinder> geometryCylinder;
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
                    ImGui::SliderFloat("_Radius",&radius,0.1f,10.0f);
                    if(ImGui::IsItemEdited()){
                        circle->setRadius(radius);
                        geometryCylinder->setRadius(radius);
                    }
                    ImGui::SliderFloat("_Height",&height,0.1f,10.0f);
                    if(ImGui::IsItemEdited()){
                        geometryCylinder->setHeight(height);
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Bottom")){
                    ImGui::SliderFloat("_Outline",&bottomOuterWidth, 0.0f, 1.0f);
                    ImGui::ColorEdit4("_InnerColor",reinterpret_cast<float*>(&bottomInnerTintColor));
                    ImGui::SliderFloat("_FlashFrequency",&bottomFlashFrequency, 0.0f, 20.0f);
                    ImGui::SliderFloat("_PatternDensity",&bottomPatternDensity, 0.0f, 10.0f);
                    ImGui::SliderFloat("_PatternWidth",&bottomPatternWidth, 0.0f, 1.0f);
                    ImGui::ColorEdit4("_PatternColor",reinterpret_cast<float*>(&bottomPatternColor));
                    ImGui::SliderInt("_PatternShape",&bottomPatternShape, 0, 7);
                    ImGui::SliderFloat("_AnimSpeed",&bottomAnimSpeed, -10.f, 10.f);
                    ImGui::ColorEdit4("_OutlineColor",reinterpret_cast<float*>(&bottomOuterTintColor));
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Wall")){
                    ImGui::ColorEdit4("_Color",reinterpret_cast<float*>(&wallTintColor));
                    ImGui::ColorEdit4("_PatternColor",reinterpret_cast<float*>(&wallPatternColor));
                    ImGui::SliderFloat("_AnimSpeed",&wallAnimSpeed,-10.0f,10.0f);
                    ImGui::SliderFloat("_PatternDensity",&wallTextureDensity,0.0f,10.0f);
                    ImGui::SliderFloat("_PatternWidth",&wallPatternWidth,0.0f,1.0f);
                    ImGui::SliderFloat("_FlashFrequency",&wallFlashFrequency,.0f,20.f);
                    ImGui::SliderFloat("_OuterWidth",&wallOuterWidth,0.0f,1.0f);
                    ImGui::SliderInt("_PatternShape",&wallPatternShape,0,3);
                    ImGui::SliderInt("_BackStyle",&wallBackStyle,0,1);
                    ImGui::Checkbox("_Fade",&wallFade);
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Bloom")){
                    ImGui::SliderFloat("blurSpeed", &blurSpeed,0.2f,10.0f);
                    // ImGui::SliderInt("blurIterations", &blurIterations,0,32);
                    ImGui::TreePop();
                }
            }
            
            ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
            ImGui::PopItemWidth();
            ImGui::End();
        }
    };

    std::shared_ptr<RenderPipelinePostProcess> areaHighlightPipeline;
    osg::ref_ptr<GaussianBlur> gaussianBlurPipeline;

    class AreaHighlightUpdateUniformCallback:public UpdateUniformCallback{
    public:
        AreaHighlightUpdateUniformCallback(std::shared_ptr<RenderPipelinePostProcess> rp):UpdateUniformCallback(rp){};
        virtual void setUniforms() override{
            const auto& renderPasses = gaussianBlurPipeline->getRenderPasses();
            for (int i = 0; i<blurIterations; i++) {
                renderPasses[i*2+1]->setUniform("_BlurSize", 1.f+i*blurSpeed);
                renderPasses[i*2+2]->setUniform("_BlurSize", 1.f+i*blurSpeed);
            }
        }
    };

    auto setupScene(){
        areaHighlightPipeline.reset(new RenderPipelinePostProcess());
        circle = new Circle(1.0f);
        osg::ref_ptr<osg::Geode> geodeCircle = new osg::Geode;
        geodeCircle->addDrawable(circle);

        std::unique_ptr<Material> materialCircle= std::make_unique<Material>(geodeCircle,"resources/shaders/highlightCircle.vert", "resources/shaders/highlightCircle.frag");

        materialCircle->addUniform("_OutlineRatio",&bottomOuterWidth);
        materialCircle->addUniform("_InnerColor",&bottomInnerTintColor);
        materialCircle->addUniform("_FlashFrequency",&bottomFlashFrequency);
        materialCircle->addUniform("_PatternDensity",&bottomPatternDensity);
        materialCircle->addUniform("_PatternWidth",&bottomPatternWidth);
        materialCircle->addUniform("_PatternColor",&bottomPatternColor);
        materialCircle->addUniform("_PatternShape",&bottomPatternShape);
        materialCircle->addUniform("_AnimSpeed",&bottomAnimSpeed);
        materialCircle->addUniform("_OutlineColor",&bottomOuterTintColor);

        geometryCylinder = new Cylinder();
        osg::ref_ptr<osg::Geode> geodeCylinder = new osg::Geode();
        geodeCylinder->addDrawable(geometryCylinder);
        std::unique_ptr<Material> materialCylinder= std::make_unique<Material>(geodeCylinder,"resources/shaders/highlightCylinder.vert", "resources/shaders/highlightCylinder.frag");

        materialCylinder->addUniform("_Color",&wallTintColor);
        materialCylinder->addUniform("_PatternColor",&wallPatternColor);
        materialCylinder->addUniform("_AnimSpeed",&wallAnimSpeed);
        materialCylinder->addUniform("_PatternDensity",&wallTextureDensity);
        materialCylinder->addUniform("_PatternWidth",&wallPatternWidth);
        materialCylinder->addUniform("_FlashFrequency",&wallFlashFrequency);
        materialCylinder->addUniform("_OuterWidth",&wallOuterWidth);
        materialCylinder->addUniform("_PatternShape",&wallPatternShape);
        materialCylinder->addUniform("_BackStyle",&wallBackStyle);
        materialCylinder->addUniform("_Fade",&wallFade);
        materialCylinder->addUniform("_Radius",&radius);
        materialCylinder->addUniform("_Height",&height);
        
        osg::ref_ptr<osg::Geode> highlightArea = new osg::Geode;
        
        highlightArea->addChild(geodeCircle);
        highlightArea->addChild(geodeCylinder);
        geodeCircle->setUpdateCallback(new UpdateUniformCallback(materialCircle.get()));
        geodeCylinder->setUpdateCallback(new UpdateUniformCallback(materialCylinder.get()));
        osg::ref_ptr<osg::BlendFunc> blend = new osg::BlendFunc;
        osg::ref_ptr<osg::Depth> depth = new osg::Depth;
        depth->setWriteMask(false);
        depth->setRange(0.0f, 0.0f);
        blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

        geodeCircle->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        geodeCylinder->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        geodeCircle->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
        geodeCircle->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
        geodeCylinder->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
        geodeCylinder->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
        geodeCircle->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
        geodeCylinder->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
        
        osg::ref_ptr<RenderTexture> buffer0 = new RenderTexture(resolution.x(),resolution.y());
        osg::ref_ptr<RenderTexture> buffer1 = new RenderTexture(resolution.x(),resolution.y());
        osg::ref_ptr<RenderTexture> screenTexture = new RenderTexture(resolution.x(),resolution.y());
        osg::ref_ptr<RTTCamera> screenRenderPass = new RTTCamera(screenTexture);
        screenRenderPass->addChildToRTTCamera(geodeCircle);
        screenRenderPass->addChildToRTTCamera(geodeCylinder);
        areaHighlightPipeline->addRenderPass(screenRenderPass);
        //bloom
        osg::ref_ptr<RTTCamera> bloomRenderPass = new RTTCamera(screenTexture,buffer0,"resources/shaders/screen.vert", "resources/shaders/bloomExtractBright.frag");
        areaHighlightPipeline->addRenderPass(bloomRenderPass);
        bloomRenderPass->addUniform("_LuminanceThreshold",luminanceThreshold);
        gaussianBlurPipeline = new GaussianBlur(buffer0,buffer1,blurIterations,blurSpeed);
        areaHighlightPipeline->addRenderPipeline(gaussianBlurPipeline);
        
        osg::ref_ptr<RTTCamera> bloomFinalPass = new RTTCamera(screenTexture,buffer0,"resources/shaders/screen.vert", "resources/shaders/bloomFinalPass.frag");
        bloomFinalPass->getDestinationQuadStateSet()->setTextureAttributeAndModes(1,static_cast<RenderTexture*>(buffer1),osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);
        bloomFinalPass->addUniform("_Bloom",1);

        areaHighlightPipeline->addRenderPass(bloomFinalPass);
        osg::ref_ptr<osg::Group> scene = new osg::Group;
        scene->setUpdateCallback(new AreaHighlightUpdateUniformCallback(areaHighlightPipeline));

        //display quad
        scene->addChild(areaHighlightPipeline->getDestinationQuadGeode());
        //off-screen rendering
        areaHighlightPipeline->addRenderPassesToOsgGroup(*scene);
        return scene;
    }

}

int main(){
    using namespace cs;
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100,resolution.x(),resolution.y());
    viewer.addEventHandler(new osgViewer::StatsHandler());
    viewer.addEventHandler(new ImGuiDemo);
    viewer.setRealizeOperation(new ImGuiInitOperation);
    auto scene = setupScene();
    
    // viewer.getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    viewer.setSceneData(scene.get());
    return viewer.run();                
}