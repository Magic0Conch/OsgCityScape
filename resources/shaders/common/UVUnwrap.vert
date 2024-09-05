#version 450 compatibility
in vec4 osg_Vertex;
in vec4 osg_MultiTexCoord0;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 _ProjectionMatrix;
uniform mat4 lightSpaceMatrix[16];
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ViewMatrix;
out vec4 lightSpacePos[16];
out vec2 texCoord;

void main(void)
{
    // gl_Position = _ProjectionMatrix*osg_ModelViewMatrix*osg_Vertex;
    gl_Position = _ProjectionMatrix*vec4(osg_MultiTexCoord0.xy, 0.0, 1.0);

    texCoord = osg_MultiTexCoord0.xy;

    for(int i = 0; i < 16; i++) {
        lightSpacePos[i] = lightSpaceMatrix[i] * osg_ViewMatrixInverse * osg_ModelViewMatrix * osg_Vertex;
        lightSpacePos[i] = lightSpacePos[i]/lightSpacePos[i].w;
        lightSpacePos[i] = lightSpacePos[i]*0.5+0.5;
        // lightSpacePos[i] = lightSpaceMatrix[i] * osg_Vertex;
    }
}