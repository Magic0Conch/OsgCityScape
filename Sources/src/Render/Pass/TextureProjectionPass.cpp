#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Texture2DArray>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Geometry>
#include <string>
#include <vector>
#include "Render/Pass/TextureProjectionPass.h"
#include "Editor/Core/RuntimeContext.h"
#include "osg/FrameBufferObject"
#include "osg/ref_ptr"
#include "Windowing/Window.h"

using namespace CSEditor::Render;

void createTextureProjectionShader(osg::StateSet* ss) {
    osg::ref_ptr<osg::Shader> vertexShader = new osg::Shader(osg::Shader::VERTEX, R"(
        #version 330 core
        #ifdef GL_ES
            precision highp float;
        #endif
        in vec4 osg_Vertex;
        in vec4 osg_MultiTexCoord0;
        // in vec3 osg_Normal;
        uniform mat4 osg_ModelViewProjectionMatrix;
        uniform mat4 lightSpaceMatrix[16];
        out vec4 lightSpacePos[16];
        out vec2 texCoord;
        // out normal;
        void main(void)
        {
            gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
            texCoord = osg_MultiTexCoord0.xy;
            // normal = transpose(inverse(mat3(model))) * osg_Normal;
            for(int i = 0; i < 16; i++) {
                lightSpacePos[i] = lightSpaceMatrix[i] * osg_Vertex;
            }
        }
    )");

    osg::ref_ptr<osg::Shader> fragmentShader = new osg::Shader(osg::Shader::FRAGMENT, R"(
        #version 330 core
        #ifdef GL_ES
            precision highp float;
        #endif
uniform sampler2DArray depthMap;
        uniform sampler2DArray colorMap;
        uniform sampler2D mainTexture;
        uniform int mapSize;
        in vec2 texCoord;
        in vec4 lightSpacePos[16];
        out vec4 fragColor;

        bool flag[16];

        vec4 projectTexture() {
            vec4 outColor = vec4(0);
            float bias = 0.005;
            int validCnt = 0;
            for(int i = 0;i<mapSize;i++){
                flag[i] = false;
            }
            for(int i = 0;i<mapSize;i++){
                vec3 projCoords = lightSpacePos[i].xyz / lightSpacePos[i].w;
                projCoords = projCoords * 0.5 + 0.5;
                if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1)
                    continue;
                float closestDepth = texture(depthMap, vec3(projCoords.xy, i)).r;
                float currentDepth = projCoords.z;
                flag[i] = currentDepth > closestDepth;

                validCnt += flag[i]? 1 : 0;                
            }
            if(validCnt == 0)
                return outColor;
            for(int i = 0;i<mapSize;i++){
                if(flag[i]){
                    // vec3 projCoords = lightSpacePos[i].xyz / lightSpacePos[i].w;
                    // projCoords = projCoords * 0.5 + 0.5;
                    // outColor += texture(colorMap, vec3(projCoords.xy, i)) / (validCnt*1.0);
                    outColor = vec4(1.0,0.0,0.0,1.0);
                }                
            }
            return outColor;
        }

        void main(void)
        {
            vec4 col = texture(mainTexture, texCoord);
            fragColor = col/2.0;
            fragColor.a = 1.0;
        }
    )");

    // 创建着色器程序
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(vertexShader.get());
    program->addShader(fragmentShader.get());
    
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

TextureProjectionPass::TextureProjectionPass(osg::ref_ptr<osg::Camera> camera):m_camera(camera) {
    m_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    m_camera->setRenderOrder(osg::Camera::PRE_RENDER);
    m_camera->setName("RenderColor");

    _texture = new osg::Texture2D();
    _texture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
    _texture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
    _texture->setSourceFormat(GL_RGBA);
    _texture->setInternalFormat(GL_RGBA32F_ARB);
    _texture->setSourceType(GL_FLOAT);

    auto stateSet = m_camera->getOrCreateStateSet();
    m_camera->setRenderOrder(osg::Camera::POST_RENDER);
    createTextureProjectionShader(stateSet);
    stateSet->addUniform(new osg::Uniform("depthMap", 1));
    stateSet->addUniform(new osg::Uniform("colorMap", 2));
    stateSet->addUniform(new osg::Uniform("mainTexture", 0));
    stateSet->addUniform(new osg::Uniform("mapSize", 0));
    m_lightSpaceMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "lightSpaceMatrix", 16);
    stateSet->addUniform(m_lightSpaceMatrixUniform);

    m_camera->attach(osg::Camera::COLOR_BUFFER0, _texture);
    Core::g_runtimeContext.windowSystem->setScreenTexture(_texture);
}


void TextureProjectionPass::setTextureArray(osg::ref_ptr<osg::Texture2DArray> depthMapArray, std::vector<osg::Texture2D *> colorTexVec, std::vector<osg::Matrixd*> projectionMatrixVec) {
    if (!m_colorMap) {
        m_colorMap = new osg::Texture2DArray;
        m_colorMap->setInternalFormat(GL_RGBA);
    }
    int cnt = colorTexVec.size();
    m_colorMap->setTextureSize(colorTexVec[0]->getTextureWidth(), colorTexVec[0]->getTextureHeight(), cnt);
    m_colorMap->setInternalFormat(colorTexVec[0]->getInternalFormat());

    for (int i = 0; i < cnt; i++) {
        m_colorMap->setImage(i, colorTexVec[i]->getImage());
    }
    auto stateSet = m_camera->getOrCreateStateSet();
    stateSet->setTextureAttributeAndModes(1, depthMapArray, osg::StateAttribute::ON);
    stateSet->setTextureAttributeAndModes(2, m_colorMap, osg::StateAttribute::ON);
    stateSet->getUniform("mapSize")->set(cnt);
    for (int i = 0; i < cnt; ++i) {
        auto& projectionMatrix = *projectionMatrixVec[i];
        m_lightSpaceMatrixUniform->setElement(i, projectionMatrix);
    }
}


void TextureProjectionPass::setTexture(osg::Texture2D * tex) {
    auto stateSet = m_camera->getOrCreateStateSet();
    stateSet->setTextureAttributeAndModes(2, tex, osg::StateAttribute::ON);
}
