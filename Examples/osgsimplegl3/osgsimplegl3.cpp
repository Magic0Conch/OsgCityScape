// This is public domain software and comes with
// absolutely no warranty. Use of public domain software
// may vary between counties, but in general you are free
// to use and distribute this software for any purpose.


// Example: OSG using an OpenGL 3.1 context.
// The comment block at the end of the source describes building OSG
// for use with OpenGL 3.x.
#include <Windows.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/GraphicsContext>
#include <osg/Camera>
#include <osg/Viewport>
#include <osg/StateSet>
#include <osg/Program>
#include <osg/Shader>
#include <osgUtil/Optimizer>

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
    
    // osg::ArgumentParser arguments( &argc, argv );

    auto root = osgDB::readNodeFile("resources/models/cow.osg");
    if( root == NULL )
    {
        osg::notify( osg::FATAL ) << "Unable to load model from command line." << std::endl;
        // return( 1 );
    }

    configureShaders( root->getOrCreateStateSet() );
    const int width( 800 ), height( 450 );
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
    if( !gc.valid() )
    {
        // osg::notify( osg::FATAL ) << "Unable to create OpenGL v" << version << " context." << std::endl;
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
    cam->setViewport( new osg::Viewport( 0, 0, width, height ) );

    viewer.setSceneData( root );
    gc->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);
    // for non GL3/GL4 and non GLES2 platforms we need enable the osg_ uniforms that the shaders will use,
    // you don't need thse two lines on GL3/GL4 and GLES2 specific builds as these will be enable by default.
    // gc->getState()->setUseModelViewAndProjectionUniforms(true);
    // gc->getState()->setUseVertexAttributeAliasing(true);

    return( viewer.run() );
}
