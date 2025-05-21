#version 450 compatibility
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D mainTexture;

void main(){
    FragColor = texture(mainTexture, TexCoords);
    FragColor = vec4(1,0,0,1);
}
