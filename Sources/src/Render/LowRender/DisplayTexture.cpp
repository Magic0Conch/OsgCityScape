#include "Render/LowRender/DisplayTexture.h"
// #include "DisplayTexture.h"
#include <osg/Geometry>
#include <osg/Geode>

using namespace CSEditor::Render;


void createDepthVisualizationShader(float near_plane, float far_plane,osg::StateSet* ss) {
    // 创建顶点着色器
    osg::ref_ptr<osg::Shader> vertexShader = new osg::Shader(osg::Shader::VERTEX, R"(
        #version 330 core
        #ifdef GL_ES
            precision highp float;
        #endif
        in vec4 osg_Vertex;
        in vec4 osg_MultiTexCoord0;
        uniform mat4 osg_ModelViewProjectionMatrix;
        out vec2 texCoord;
        void main(void)
        {
            gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
            texCoord = osg_MultiTexCoord0.xy;
        }
    )");

    // 创建片段着色器
    osg::ref_ptr<osg::Shader> fragmentShader = new osg::Shader(osg::Shader::FRAGMENT, R"(
        #version 330 core
        // gl3_FragmentShader
        #ifdef GL_ES
            precision highp float;
        #endif
        uniform sampler2D baseTexture;
        uniform float near_plane;
        uniform float far_plane;
        in vec2 texCoord;
        out vec4 color;

        float LinearizeDepth(float depth) {
            float z = depth * 2.0 - 1.0; // Back to NDC 
            return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
        }

        void main(void)
        {
            float depthValue = texture(baseTexture, texCoord).r;
            float linearDepth = LinearizeDepth(depthValue) / far_plane; // Normalize to [0, 1]
            color = vec4(vec3(linearDepth), 1.0);
        }
    )");

    // 创建着色器程序
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(vertexShader.get());
    program->addShader(fragmentShader.get());

    // 设置uniform变量
    osg::ref_ptr<osg::Uniform> nearPlaneUniform = new osg::Uniform("near_plane", near_plane);
    osg::ref_ptr<osg::Uniform> farPlaneUniform = new osg::Uniform("far_plane", far_plane);

    ss->addUniform(nearPlaneUniform.get());
    ss->addUniform(farPlaneUniform.get());
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

osg::Geode * createFullscreenQuad() {
     osg::Geometry* geom = osg::createTexturedQuadGeometry(osg::Vec3(), osg::Vec3(1, 0.0f, 0.0f), osg::Vec3(0.0f, 1, 0.0f), 0.0f, 0.0f, 1.0f, 1.0f);
     osg::Geode *quad = new osg::Geode;
     quad->addDrawable(geom);
     return quad;
 }


DisplayTexture::DisplayTexture()
{
    setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    setRenderOrder(osg::Camera::POST_RENDER);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setViewMatrix(osg::Matrix());
    setProjectionMatrix(osg::Matrix::ortho2D(0, 1, 0, 1));
    // getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF);
    createDepthVisualizationShader(0.1f, 1000.0f, getOrCreateStateSet());
    addChild(createFullscreenQuad());

    setName("Display");

}

void DisplayTexture::setTexture(osg::Texture2D *tex)
{
     getOrCreateStateSet()->setTextureAttributeAndModes(0, tex,osg::StateAttribute::ON);
}
