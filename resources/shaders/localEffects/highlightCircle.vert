#version 330 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoords;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 _ProjectionMatrix;

out VS_OUT{
    lowp vec2 uv;
    mediump vec3 localPos;
}vs_out;

void main(){
    gl_Position = _ProjectionMatrix*osg_ModelViewMatrix*aPos;
    vs_out.uv = aTexCoords;
    vs_out.localPos = aPos.xyz;
}
