#version 330 core
in vec4 osg_Vertex;
in vec4 osg_MultiTexCoord0;
uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 lightSpaceMatrix[16];
out vec4 lightSpacePos[16];
out vec2 texCoord;

void main(void)
{
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
    texCoord = osg_MultiTexCoord0.xy;

    for(int i = 0; i < 16; i++) {
        lightSpacePos[i] = lightSpaceMatrix[i] * osg_Vertex;
    }
}