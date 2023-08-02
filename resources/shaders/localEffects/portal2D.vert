#version 450 compatibility
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
void main(){
    gl_Position = vec4(aPos.xy,0,1.0);
    TexCoords = aTexCoords;
}