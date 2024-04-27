#version 330 core
uniform lowp vec4 _OutlineColor;
out vec4 FragColor;

void main(){
    FragColor = _OutlineColor;
}