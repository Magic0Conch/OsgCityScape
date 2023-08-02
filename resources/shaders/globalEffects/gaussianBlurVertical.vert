#version 330 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 uvs[5];
uniform float _BlurSize;
uniform vec2 _MainTex_TexelSize;

void main(){
    gl_Position = vec4(aPos.xy,0,1.0);
    uvs[0]=aTexCoords;
    uvs[1]=aTexCoords + vec2(_MainTex_TexelSize.x*1.0,0.0)*_BlurSize;
    uvs[2]=aTexCoords - vec2(_MainTex_TexelSize.x*1.0,0.0)*_BlurSize;
    uvs[3]=aTexCoords + vec2(_MainTex_TexelSize.x*2.0,0.0)*_BlurSize;
    uvs[4]=aTexCoords - vec2(_MainTex_TexelSize.x*2.0,0.0)*_BlurSize;;
}