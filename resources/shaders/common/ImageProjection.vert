#version 450 compatibility
in vec4 osg_Vertex;
in vec4 osg_MultiTexCoord0;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 _ProjectionMatrix;
uniform mat4 lightSpaceMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ViewMatrix;
out vec4 lightSpacePos;
out vec2 texCoord;

void main(void)
{
    gl_Position = _ProjectionMatrix*osg_ModelViewMatrix*osg_Vertex;
    texCoord = osg_MultiTexCoord0.xy;

    lightSpacePos = lightSpaceMatrix * osg_ViewMatrixInverse * osg_ModelViewMatrix * osg_Vertex;
    lightSpacePos = lightSpacePos/lightSpacePos.w;
    lightSpacePos = lightSpacePos*0.5+0.5;
}