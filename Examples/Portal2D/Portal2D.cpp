#include <windows.h>
#include "Render/Movie.h"
#include "Render/RTTCamera.h"
#include "Render/RenderTexture.h"
#include "UI/OsgImGuiHandler.h"
#include "UI/ImGuiInitOperation.h"
#include "Render/RenderPipelinePostProcess.h"
#include "Render/Effects/GaussianBlur.h"
#include "Utils/ShaderUtils.h"
#include "osg/ref_ptr"


const int width = 1728;
const int height = 702;

//public parameters
float emitCenterUVX = 0.582f;
float effectOuterWidth = 0.099f;
float effectInnerWidth = 0.00f;
float emitCenterUVY=.243f;
float effectOuterHeight=.337f;

//blur parameters
float blurSpeed = 1.6f;
int blurIterations = 24;

//wave parameters
osg::Vec4 waveColor = osg::Vec4(0.5838302,0.9150943,0.8609131,0.3);
float amplitude = .871f;
float frequency = 0.149f;
float waveWidth = .434f;
float waveSpeed = .414f;
float seamWidth = .066f;
bool isHorizontal = false;

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
            if(ImGui::Begin("Properties",&p_open,window_flags)){
                if (ImGui::TreeNode("Transform Properties")){
                    ImGui::SliderFloat("emitCenterUVX", &emitCenterUVX, 0.0f, 1.0f);
                    ImGui::SliderFloat("effectOuterWidth", &effectOuterWidth, 0.0f, 1.0f);
                    ImGui::SliderFloat("effectInnerWidth", &effectInnerWidth, 0.0f, 1.0f);
                    ImGui::SliderFloat("emitCenterUVY", &emitCenterUVY, 0.0f, 1.0f);
                    ImGui::SliderFloat("effectOuterHeight", &effectOuterHeight, 0.0f, 1.0f);
                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Wave Properties")){
                    ImGui::ColorEdit4("waveColor", (float*)&waveColor);
                    ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 1.0f);
                    ImGui::SliderFloat("frequency", &frequency, 0.0f, 1.0f);
                    ImGui::SliderFloat("waveWidth", &waveWidth, 0.0f, 1.0f);
                    ImGui::SliderFloat("waveSpeed", &waveSpeed, 0.0f, 10.0f);
                    ImGui::SliderFloat("seamWidth", &seamWidth, 0.0f, 1.0f);
                    ImGui::Checkbox("isHorizontal", &isHorizontal);
                    ImGui::TreePop();
                }
                if(ImGui::TreeNode("Blur Properties")){
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

    std::shared_ptr<RenderPipelinePostProcess> portal2DPipeline;
    osg::ref_ptr<GaussianBlur> gaussianBlurPipeline;

    class Portal2DUpdateUniformCallback:public UpdateUniformCallback{
    public:
        Portal2DUpdateUniformCallback(std::shared_ptr<RenderPipelinePostProcess> rp):UpdateUniformCallback(rp){};
        virtual void setUniforms() override{
            const auto& x = portal2DPipeline->getRenderPasses();
            const auto& renderPasses = gaussianBlurPipeline->getRenderPasses();
            for (int i = 0; i<blurIterations; i++) {
                renderPasses[i*2+1]->setUniform("_BlurSize", 1.f+i*blurSpeed);
                renderPasses[i*2+2]->setUniform("_BlurSize", 1.f+i*blurSpeed);
            }
        }
    };
    auto setupScene(){
        portal2DPipeline.reset(new RenderPipelinePostProcess());
        //set up a quad that displays a video.(Scene)
        auto movie = new Movie("resources/videos/video1_mp4");
        osg::ref_ptr<osg::Geode> geodeMovieQuad = new osg::Geode;
        auto geomMovieQuad = movie->getScreenQuadGeometry();
        geomMovieQuad->setUpdateCallback(new MovieCallback(movie));
        geodeMovieQuad->addDrawable(geomMovieQuad);

        //pass0: render scene to a quad.
        osg::ref_ptr<RenderTexture> buffer0 = new RenderTexture(width,height);
        portal2DPipeline->addRenderPass(new RTTCamera(geodeMovieQuad,buffer0,"resources/shaders/screen.vert", "resources/shaders/screen.frag"));
        
        //Pass1 & 2: gaussian blur
        osg::ref_ptr<RenderTexture> buffer1 = new RenderTexture(width,height);
        gaussianBlurPipeline = new GaussianBlur(buffer0,buffer1,width,height,blurIterations,blurSpeed); 
        portal2DPipeline->addRenderPipeline(gaussianBlurPipeline);
        
        //Pass 3:wave pass
        const auto wavePass = new RTTCamera(buffer1,buffer0,
        "resources/shaders/portal2D.vert","resources/shaders/portal2D.frag");
        wavePass->addUniform("_EmitCenterUVX", &emitCenterUVX);
        wavePass->addUniform("_EffectOuterWidth", &effectOuterWidth);
        wavePass->addUniform("_EffectInnerWidth", &effectInnerWidth);
        wavePass->getDestinationQuadStateSet()->setTextureAttributeAndModes(1,static_cast<RenderTexture*>(movie->getFrame().get()),
        osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);        
        wavePass->addUniform("_OriginalTexture",1);
        wavePass->addUniform("_EmitCenterUVY",&emitCenterUVY);
        wavePass->addUniform("_EffectOuterHeight",&effectOuterHeight);
        wavePass->addUniform("_WaveColor",&waveColor);
        wavePass->addUniform("_Amplitude", &amplitude);
        wavePass->addUniform("_Frequency", &frequency);
        wavePass->addUniform("_WaveWidth", &waveWidth);
        wavePass->addUniform("_WaveSpeed", &waveSpeed);
        wavePass->addUniform("_SeamWidth", &seamWidth);
        wavePass->addUniform("_IsHorizontal", &isHorizontal);        
        portal2DPipeline->addRenderPass(wavePass);
        
        //composite a scene root
        osg::ref_ptr<osg::Group> scene = new osg::Group;
        //display quad
        scene->addChild(portal2DPipeline->getDestinationQuadGeode());
        //off-screen rendering
        portal2DPipeline->addRenderPassesToOsgGroup(*scene);
        return scene;
    }
}

int main(){
    using namespace cs;
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100,width, height);
    viewer.addEventHandler(new osgViewer::StatsHandler());
    viewer.addEventHandler(new ImGuiDemo);
    viewer.setRealizeOperation(new ImGuiInitOperation);
    auto scene = setupScene();
    scene->setUpdateCallback(new Portal2DUpdateUniformCallback(portal2DPipeline));
    viewer.setSceneData(scene.get());
    // viewer.getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    //     viewer.getCamera()->getGraphicsContext()->getState()->setUseVertexAttributeAliasing(true);
    return viewer.run();
}