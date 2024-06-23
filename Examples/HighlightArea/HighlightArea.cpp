#include <memory>
#include <windows.h>
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/UIManager.h"
#include "Render/Entities/Circle.h"
#include "Render/Entities/Cylinder.h"
#include "Render/Effects/Bloom.h"
#include "Render/Effects/GaussianBlur.h"
#include "GUI/Helper/ImGuiInitOperation.h"
#include "imgui.h"
#include "osg/Geode"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "osg/StateAttribute"
#include "osg/Vec2i"
#include "osg/Vec4"
#include "osg/ref_ptr"
#include "osg/Depth"
#include <iostream>
#include <osgViewer/Viewer>
#include "GUI/Core/Panel.h"
#include <osg/BlendFunc>
#include <osg/CullFace>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/View>
#include "Core/ECS/Components/Material.h"
const auto resolution = osg::Vec2i(1728,702);

float radius=1.0f;

float height=2.0f;
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

//bloom
int blurIterations = 4;
float blurSpeed = 0.6f;
int downSample = 2;
float luminanceThreshold = 0.4f;

namespace CSEditor {
    osg::ref_ptr<Render::Circle> circle;
    osg::ref_ptr<Render::Cylinder> geometryCylinder;
    class PanelProperties : public GUI::Panel
    {
    protected:
        virtual void drawImpl() override{
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

    std::shared_ptr<Render::RenderPipelinePostProcess> areaHighlightPipeline;
    osg::ref_ptr<Render::Bloom> bloomPipeline;

    class BloomUniformCallback:public Render::UpdateUniformCallback{
    public:
        BloomUniformCallback(std::shared_ptr<Render::RenderPipelinePostProcess> rp):UpdateUniformCallback(rp){};
        virtual void setUniforms() override{
            const auto& renderPasses = bloomPipeline->getGaussianPipeline()->getRenderPasses();
            for (int i = 0; i<blurIterations; i++) {
                renderPasses[i*2+1]->getMaterial()->setUniform("_BlurSize", 1.f+i*blurSpeed);
                renderPasses[i*2+2]->getMaterial()->setUniform("_BlurSize", 1.f+i*blurSpeed);
            }
        }
    };

    auto setupScene(){
        // areaHighlightPipeline.reset(new Render::RenderPipelinePostProcess());
        circle = new Render::Circle(1.0f);
        // osg::ref_ptr<osg::Geode> geodeCircle = new osg::Geode;
        // geodeCircle->addDrawable(circle);
        const std::string& vertCirclePath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCircle.vert").string();
        const std::string& fragCirclePath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCircle.frag").string();

        auto materialCircle= std::make_unique<ECS::Material>(vertCirclePath,fragCirclePath);
        materialCircle->bind(static_cast<osg::ref_ptr<osg::Geometry>>(circle));

        materialCircle->setFloat("_OutlineRatio",bottomOuterWidth);
        materialCircle->setVec4("_InnerColor",bottomInnerTintColor);
        materialCircle->setFloat("_FlashFrequency",bottomFlashFrequency);
        materialCircle->setFloat("_PatternDensity",bottomPatternDensity);
        materialCircle->setFloat("_PatternWidth",bottomPatternWidth);
        materialCircle->setVec4("_PatternColor",bottomPatternColor);
        materialCircle->setInt("_PatternShape",bottomPatternShape);
        materialCircle->setFloat("_AnimSpeed",bottomAnimSpeed);
        materialCircle->setVec4("_OutlineColor",bottomOuterTintColor);

        geometryCylinder = new Render::Cylinder();
        // osg::ref_ptr<osg::Geode> geodeCylinder = new osg::Geode();
        // geodeCylinder->addDrawable(geometryCylinder);
        const std::string& vertCylinderPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCylinder.vert").string();
        const std::string& fragCylinderPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/highlightCylinder.frag").string();

        std::unique_ptr<ECS::Material> materialCylinder= std::make_unique<ECS::Material>(vertCylinderPath,fragCylinderPath);
        materialCylinder->bind(static_cast<osg::ref_ptr<osg::Geometry>>(geometryCylinder));

        materialCylinder->setVec4("_Color",wallTintColor);
        materialCylinder->setVec4("_PatternColor",wallPatternColor);
        materialCylinder->setFloat("_AnimSpeed",wallAnimSpeed);
        materialCylinder->setFloat("_PatternDensity",wallTextureDensity);
        materialCylinder->setFloat("_PatternWidth",wallPatternWidth);
        materialCylinder->setFloat("_FlashFrequency",wallFlashFrequency);
        materialCylinder->setFloat("_OuterWidth",wallOuterWidth);
        materialCylinder->setInt("_PatternShape",wallPatternShape);
        materialCylinder->setInt("_BackStyle",wallBackStyle);
        materialCylinder->setInt("_Fade",wallFade);
        materialCylinder->setFloat("_Radius",radius);
        materialCylinder->setFloat("_Height",height);
        
        osg::ref_ptr<osg::Geode> highlightArea = new osg::Geode;
        
        highlightArea->addChild(geometryCylinder);
        highlightArea->addChild(circle);
        // geodeCircle->setUpdateCallback(new Render::UpdateUniformCallback(materialCircle.get()));
        // geodeCylinder->setUpdateCallback(new Render::UpdateUniformCallback(materialCylinder.get()));
        
        osg::ref_ptr<osg::BlendFunc> blend = new osg::BlendFunc;
        osg::ref_ptr<osg::Depth> depth = new osg::Depth;
        depth->setWriteMask(false);
        // depth->setRange(0.0f, 0.0f);
        blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

        materialCircle->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        materialCircle->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
        materialCircle->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
        materialCircle->getStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);

        materialCylinder->getStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        materialCylinder->getStateSet()->setAttributeAndModes(blend.get(),osg::StateAttribute::ON);
        materialCylinder->getStateSet()->setAttributeAndModes(depth.get(),osg::StateAttribute::ON);
        materialCylinder->getStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
        
        //render geode to a screen texture
        osg::ref_ptr<Render::RenderTexture> screenTexture = new Render::RenderTexture(resolution.x(),resolution.y());
        // osg::ref_ptr<Render::RTTCamera> screenRenderPass = new Render::RTTCamera(screenTexture);
        // screenRenderPass->addChildToRTTCamera(geodeCircle);
        // screenRenderPass->addChildToRTTCamera(geodeCylinder);
        // areaHighlightPipeline->addRenderPass(screenRenderPass);
        
        //bloom from a screen texture
        // bloomPipeline = new Render::Bloom(screenTexture,nullptr,blurIterations,blurSpeed,&luminanceThreshold);
        // areaHighlightPipeline->addRenderPipeline(bloomPipeline);

        osg::ref_ptr<osg::Group> scene = new osg::Group;
        // scene->setUpdateCallback(new BloomUniformCallback(areaHighlightPipeline));

        //display quad
        scene->addChild(highlightArea);
        //off-screen rendering
        // areaHighlightPipeline->addRenderPassesToOsgGroup(*scene);
        return scene;
    }

}

int main(){
    using namespace CSEditor;
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100,resolution.x(),resolution.y());
    viewer.addEventHandler(new osgViewer::StatsHandler());
    // viewer.addEventHandler(new GUI::UIManager);
    viewer.setRealizeOperation(new GUI::ImGuiInitOperation);
    auto scene = setupScene();
    
    // viewer.getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    viewer.setSceneData(scene.get());
    return viewer.run();                
}