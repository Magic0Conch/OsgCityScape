#include <Windows.h>
#include <iostream>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/GraphicsContext>
#include <osg/Camera>
#include <osg/Viewport>
#include <osg/StateSet>
#include <osg/Program>
#include <osg/Shader>
#include <osgUtil/Optimizer>
#include <ostream>
#include "Editor/Core/RuntimeContext.h"
#include "Utils/OsgconvWrapper.h"

void configureShaders( osg::StateSet* stateSet )
{
    const std::string vertexSource =
        "#version 330 \n"
        " \n"
        "uniform mat4 osg_ModelViewProjectionMatrix; \n"
        "uniform mat3 osg_NormalMatrix; \n"
        "uniform vec3 ecLightDir; \n"
        " \n"
        "in vec4 osg_Vertex; \n"
        "in vec3 osg_Normal; \n"
        "out vec4 color; \n"
        " \n"
        "void main() \n"
        "{ \n"
        "    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); \n"
        "    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); \n"
        "    color = vec4( vec3( diffuse ), 1. ); \n"
        " \n"
        "    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; \n"
        "} \n";
    osg::Shader* vShader = new osg::Shader( osg::Shader::VERTEX, vertexSource );

    const std::string fragmentSource =
        "#version 330 \n"
        " \n"
        "in vec4 color; \n"
        "out vec4 fragData; \n"
        " \n"
        "void main() \n"
        "{ \n"
        "    fragData = color; \n"
        "} \n";
    osg::Shader* fShader = new osg::Shader( osg::Shader::FRAGMENT, fragmentSource );

    osg::Program* program = new osg::Program;
    program->addShader( vShader );
    program->addShader( fShader );
    stateSet->setAttribute( program );

    osg::Vec3f lightDir( 0., 0.5, 1. );
    lightDir.normalize();
    stateSet->addUniform( new osg::Uniform( "ecLightDir", lightDir ) );
}

int main( int argc, char** argv )
{
    // std::string filename = "E:\\work\\github\\OsgCityScape\\resources\\materials\\wall.png";
    // osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);
    // if (!image.valid()) {
    //     // 处理加载失败的情况
    //     std::cerr << "Failed to load image: " << filename << std::endl;
    //     int x;
    //     std::cin>>x;
    // }
    // return 0;

    // const auto& binFolder = CSEditor::Core::g_runtimeContext.configManager->getBinaryFolder();
    std::string inputFile = "C:\\suizhou\\GovFacility\\Data\\Tile_+000_+003\\Tile_+000_+003.obj";
    std::string outputFile = "C:\\suizhou\\GovFacility\\Data\\Tile_+000_+003\\Tile_+000_+003.ive";
    CSEditor::Dialog::OsgconvWrapper::callOsgconv(inputFile, outputFile);

    auto root = osgDB::readNodeFile("C:/suizhou/GovFacility/Data/Tile_+000_+003/Tile_+000_+003.ive");
    if( root == NULL ){
        osg::notify( osg::FATAL ) << "Unable to load model from command line." << std::endl;
    }
    // configureShaders(root->getOrCreateStateSet());
    const int width(800 ), height( 450 );
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
    traits->x = 20; traits->y = 30;
    traits->width = width; traits->height = height;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->samples = 4;
    traits->readDISPLAY();
    traits->setUndefinedScreenDetailsToDefaultScreen();
    traits->glContextProfileMask = 0x1;// 0x1;// 
    osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
    if(!gc.valid()){
        return( 1 );
    }
    gc->getState()->resetVertexAttributeAlias(false);
    osgViewer::Viewer viewer;

    // Create a Camera that uses the above OpenGL context.
    osg::Camera* cam = viewer.getCamera();
    cam->setGraphicsContext( gc.get() );
    // Must set perspective projection for fovy and aspect.
    cam->setProjectionMatrix( osg::Matrix::perspective( 30., (double)width/(double)height, 1., 100. ) );
    // Unlike OpenGL, OSG viewport does *not* default to window dimensions.
    cam->setViewport(new osg::Viewport(0,0,width,height));

    viewer.setSceneData(root);
    gc->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);

    return( viewer.run() );
}
