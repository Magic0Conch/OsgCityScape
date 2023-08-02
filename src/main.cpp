#include <iostream>
#include <vector>
#include <windows.h>
#include "Movie.h"
#include "RTTCamera.h"
#include "RenderTexture.h"
#include <osg/Group>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osg/Uniform>
#include "osg/Notify"
#include "osg/Vec2f"
#include "osg/Vec3"
#include "osgDB/AuthenticationMap"
#include "osgDB/Registry"
#include "utils/SearchGeodeNode.h"
#include "utils/ShaderUtils.h"
#include <osg/Geode>
#include <osg/ref_ptr>
#include <osg/Vec4>
#include <osg/Program>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osgDB/Archive>
#include <osgViewer/ViewerEventHandlers>

const int width = 1728;
const int height = 702;

//public parameters
float emitCenterUVX = 0.582f;
float effectOutterWidth = 0.099f;
float effectInnerWidth = 0.00f;
float emitCenterUVY=.243f;
float effectOutterHeight=.337f;

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
    struct Portal2DPipeline{
        osg::ref_ptr<RTTCamera> pass0;
        std::vector<osg::ref_ptr<RTTCamera>> gaussianBlurPasses;
        osg::ref_ptr<RTTCamera> wavePass;
    }portal2DPipeline;

    auto setupScene(){
        //set up a quad that displays a video.(Scene)
        auto movie = new Movie("resources/videos/video1_mp4");
        osg::ref_ptr<osg::Geode> geodeMovieQuad = new osg::Geode;
        auto geomMovieQuad = movie->getScreenQuadGeometry();
        geomMovieQuad->setUpdateCallback(new MovieCallback(movie));
        geodeMovieQuad->addDrawable(geomMovieQuad);
    
        
        //pass0: render scene to a quad.
        osg::ref_ptr<RenderTexture> buffer0 = new RenderTexture(width,height);
        portal2DPipeline.pass0 = new RTTCamera(geodeMovieQuad,buffer0,"resources/shaders/screen.vert", "resources/shaders/screen.frag");
        
        //Pass1 & 2: gaussian blur
        const auto texelSize = new osg::Uniform("_MainTex_TexelSize",osg::Vec2f(1./width,1./height));
        osg::ref_ptr<RenderTexture> buffer1 = new RenderTexture(width,height);
        for (int i = 0; i<blurIterations; i++) {
            //vertical pass
            osg::ref_ptr<RTTCamera> verticalPass = new RTTCamera(buffer0,buffer1,
            "resources/shaders/gaussianBlurVertical.vert", "resources/shaders/gaussianBlur.frag");
            verticalPass->addUniform(new osg::Uniform("_BlurSize",1.f+i*blurSpeed));
            verticalPass->addUniform(texelSize);
            portal2DPipeline.gaussianBlurPasses.emplace_back(verticalPass);
            //horizontal pass
            osg::ref_ptr<RTTCamera> horizontalPass = new RTTCamera(buffer1,buffer0,
            "resources/shaders/gaussianBlurHorizontal.vert", "resources/shaders/gaussianBlur.frag");
            horizontalPass->addUniform(new osg::Uniform("_BlurSize",1.f+i*blurSpeed));
            horizontalPass->addUniform(texelSize);
            portal2DPipeline.gaussianBlurPasses.emplace_back(horizontalPass);
        }
        
        //Pass 3:wave pass
        portal2DPipeline.wavePass = new RTTCamera(buffer0,buffer1,
        "resources/shaders/portal2D.vert","resources/shaders/portal2D.frag");
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_EmitCenterUVX", emitCenterUVX));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_EffectOutterWidth", effectOutterWidth));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_EffectInnerWidth", effectInnerWidth));
        portal2DPipeline.wavePass->getDestinationQuadGeode()->getStateSet()->setTextureAttributeAndModes(1,static_cast<RenderTexture*>(movie->getFrame().get()),
        osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);
        
        portal2DPipeline.wavePass->addUniform(new osg::Uniform("_OriginalTexture",1));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_EmitCenterUVY",emitCenterUVY));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_EffectOutterHeight",effectOutterHeight));
        
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_WaveColor",waveColor));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_Amplitude", amplitude));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_Frequency", frequency));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_WaveWidth", waveWidth));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_WaveSpeed", waveSpeed));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_SeamWidth", seamWidth));
        portal2DPipeline.wavePass->addUniform(new osg::Uniform ("_IsHorizontal", isHorizontal));

        //composite a scene root
        osg::ref_ptr<osg::Group> scene = new osg::Group;
        //display quad
        scene->addChild(portal2DPipeline.wavePass->getDestinationQuadGeode());
        //off-screen rendering
        scene->addChild(portal2DPipeline.pass0);
        for(const auto pass:portal2DPipeline.gaussianBlurPasses){
            scene->addChild(pass);
        }
        scene->addChild(portal2DPipeline.wavePass);
        return scene;
    }

}

int main(){
    using namespace cs;
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100,width, height);
    viewer.addEventHandler(new osgViewer::StatsHandler());
    auto scene = setupScene();
    viewer.setSceneData(scene.get());
    return viewer.run();
}