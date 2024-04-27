#version 330 compatibility
layout(location = 0)in vec4 aPos;
layout(location = 1)in vec2 aTexCoords;
uniform mat4 osg_ModelViewProjectionMatrix;
in vec4 osg_MultiTexCoord0;

out vec2 TexCoords;

void main(){
    gl_Position = osg_ModelViewProjectionMatrix*aPos;
    TexCoords = osg_MultiTexCoord0.xy;
}

