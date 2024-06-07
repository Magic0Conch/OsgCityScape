#version 450 compatibility
in vec2 TexCoords;
out vec4 FragColor;
uniform vec4 _Color;

void main(){
    FragColor = _Color;
}