#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Texture2DArray>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Geometry>
#include <vector>
#include "Render/Pass/TextureProjectionPass.h"


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
                float closestDepth = texture(depthMap, vec3(projCoords.xy, i)).r;
                float currentDepth = projCoords.z;
                flag[i] = currentDepth - bias > closestDepth;

                validCnt += flag[i]? 1 : 0;                
            }
            if(validCnt == 0)
                return outColor;
            for(int i = 0;i<mapSize;i++){
                if(flag[i]){
                    vec3 projCoords = lightSpacePos[i].xyz / lightSpacePos[i].w;
                    projCoords = projCoords * 0.5 + 0.5;
                    outColor += texture(colorMap, vec3(projCoords.xy, i)) / (validCnt*1.0);
                }                
            }
            return outColor;
        }

        void main(void)
        {
            fragColor = projectTexture();
        }
    )");

    // 创建着色器程序
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(vertexShader.get());
    program->addShader(fragmentShader.get());
    
    // 设置uniform变量
    // osg::ref_ptr<osg::Uniform> nearPlaneUniform = new osg::Uniform("near_plane", near_plane);
    // osg::ref_ptr<osg::Uniform> farPlaneUniform = new osg::Uniform("far_plane", far_plane);

    // ss->addUniform(nearPlaneUniform.get());
    // ss->addUniform(farPlaneUniform.get());
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

TextureProjectionPass::TextureProjectionPass() {
    auto stateSet = getOrCreateStateSet();
    createTextureProjectionShader(stateSet);
    stateSet->addUniform(new osg::Uniform("depthMap", 0));
    stateSet->addUniform(new osg::Uniform("colorMap", 1));
    stateSet->addUniform(new osg::Uniform("mapSize", 0));
    m_lightSpaceMatrixUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "lightSpaceMatrix", 16);
    stateSet->addUniform(m_lightSpaceMatrixUniform);
}

void TextureProjectionPass::setTextureArray(std::vector<osg::Texture2D*>  depthTexVec, std::vector<osg::Texture2D *> colorTexVec, std::vector<osg::Matrixd*> projectionMatrixVec) {
    if (!m_depthMap) {
        m_depthMap = new osg::Texture2DArray;
        m_depthMap->setSourceType(GL_FLOAT);
    }
    if (!m_colorMap) {
        m_colorMap = new osg::Texture2DArray;
        m_colorMap->setInternalFormat(GL_RGBA);
    }
    int cnt = depthTexVec.size();
    m_depthMap->setTextureSize(colorTexVec[0]->getTextureWidth(), colorTexVec[0]->getTextureWidth(), cnt);
    m_colorMap->setTextureSize(colorTexVec[0]->getTextureWidth(), colorTexVec[0]->getTextureHeight(), cnt);
    m_colorMap->setInternalFormat(colorTexVec[0]->getInternalFormat());
    
    for (int i = 0; i < cnt; i++) {
        m_depthMap->setImage(i, depthTexVec[i]->getImage());
        m_colorMap->setImage(i, colorTexVec[i]->getImage());
    }
    auto stateSet = getOrCreateStateSet();
    stateSet->setTextureAttributeAndModes(0, m_depthMap, osg::StateAttribute::ON);
    stateSet->setTextureAttributeAndModes(1, m_colorMap, osg::StateAttribute::ON);
    stateSet->getUniform("mapSize")->set(cnt);
    for (int i = 0; i < cnt; ++i) {
        auto& projectionMatrix = *projectionMatrixVec[i];
        m_lightSpaceMatrixUniform->setElement(i, projectionMatrix);
    }
}


