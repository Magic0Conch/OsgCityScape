#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D _MainTex;
void main(){
    vec3 mainTexColor = texture(_MainTex,TexCoords).rgb;
    FragColor = vec4(mainTexColor, 1.0);
}