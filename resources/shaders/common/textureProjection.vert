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