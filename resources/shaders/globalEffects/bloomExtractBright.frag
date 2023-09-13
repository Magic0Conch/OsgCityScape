#version 440 compatibility

uniform sampler2D _MainTex;
uniform float _LuminanceThreshold;

in vec2 TexCoords;
out vec4 FragColor;

float luminance(vec3 col){
    return col.r * 0.2125 + col.g * 0.7154 + col.b * 0.0721;
}

void main(){
    vec4 texColor = texture(_MainTex,TexCoords);
    float brightness = luminance(texColor.rgb);
    float coeff = clamp(brightness - _LuminanceThreshold,0.0,1.0);
    FragColor = texColor*coeff;
}