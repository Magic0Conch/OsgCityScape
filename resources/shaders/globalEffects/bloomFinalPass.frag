#version 440 compatibility

uniform sampler2D _MainTex;
uniform sampler2D _Bloom;
in vec2 TexCoords;
out vec4 FragColor;

void main(){
    vec3 texColor = texture2D(_MainTex,TexCoords).rgb;
    vec3 bloomColor = texture2D(_Bloom,TexCoords).rgb;
    FragColor = vec4(texColor + bloomColor,1.0);
}