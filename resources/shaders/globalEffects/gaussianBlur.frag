#version 330 core
in vec2 uvs[5];
out vec4 FragColor;
uniform sampler2D _MainTex;

float weight[3] = float[](0.4026,0.2442,0.0545);

void main(){
    vec3 sum = texture(_MainTex,uvs[0]).rgb*weight[0];
    for(int iter= 1;iter<3;iter++){
        sum+=texture(_MainTex,uvs[iter]).rgb * weight[iter];
        sum+=texture(_MainTex,uvs[iter*2]).rgb * weight[iter];
    }
    FragColor = vec4(sum, 1.0);
}