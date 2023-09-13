#version 450 compatibility
layout(location = 0)in vec4 aPos;
layout(location = 1)in vec2 aTexCoords;
uniform mat4 osg_ModelViewProjectionMatrix;

out vec2 TexCoords;

void main(){
    gl_Position = osg_ModelViewProjectionMatrix*aPos;
    TexCoords = aTexCoords;
}

