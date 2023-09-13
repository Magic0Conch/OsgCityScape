#version 330 core
in vec2 uvs[5];
out vec4 FragColor;
uniform sampler2D _MainTex;

float weight[3] = float[](0.4026,0.2442,0.0545);

void main(){
    vec3 sum = texture(_MainTex,uvs[0]).rgb*weight[0];
    sum+=texture(_MainTex,uvs[1]).rgb * weight[1];
    sum+=texture(_MainTex,uvs[2]).rgb * weight[1];
    sum+=texture(_MainTex,uvs[3]).rgb * weight[2];
    sum+=texture(_MainTex,uvs[4]).rgb * weight[2];
    FragColor = vec4(sum, 1.0);
}