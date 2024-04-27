#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D mainTexture;

void main(){
    vec4 col = texture(mainTexture, TexCoords);
    FragColor = col;
}