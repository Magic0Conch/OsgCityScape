#version 330 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoords;
uniform mat4 osg_ModelViewProjectionMatrix;
uniform bool _Fade;

out VS_OUT{
    lowp vec2 uv;
    lowp float alpha;
    mediump vec3 localPos;
}vs_out;

void main(){
    gl_Position = osg_ModelViewProjectionMatrix*aPos;
    vs_out.uv = aTexCoords;
    vs_out.localPos = aPos.xyz;
    float a = clamp(1-vs_out.uv.y,0.0,1.0);                
    vs_out.alpha =_Fade? a:1.0;
}