#version 450 compatibility
uniform lowp vec4 _OutlineColor;
out vec4 FragColor;

void main(){
    FragColor = vec4(_OutlineColor.rgb);
}