#version 330 core
#ifdef GL_ES
    precision highp float;
#endif
uniform sampler2DArray depthMap;
uniform sampler2DArray colorMap;
uniform int mapSize;
in vec2 texCoord;
in vec4 lightSpacePos[16];
out vec4 fragColor;

bool flag[16];

vec4 projectTexture() {
    vec4 outColor = vec4(0);
    float bias = 0.005;
    int validCnt = 0;
    for(int i = 0;i<mapSize;i++){
        flag[i] = false;
    }
    for(int i = 0;i<mapSize;i++){
        vec3 projCoords = lightSpacePos[i].xyz / lightSpacePos[i].w;
        projCoords = projCoords * 0.5 + 0.5;
        float closestDepth = texture(depthMap[i], vec3(projCoords.xy, i)).r;
        float currentDepth = projCoords.z;
        flag[i] = currentDepth - bias > closestDepth;

        vaildCnt += flag[i]? 1 : 0;                
    }
    if(validCnt == 0)
        return outColor;
    for(int i = 0;i<mapSize;i++){
        if(flag[i]){
            vec3 projCoords = lightSpacePos[i].xyz / lightSpacePos[i].w;
            projCoords = projCoords * 0.5 + 0.5;
            outColor += texture(colorMap[i], vec3(projCoords.xy, i)) / (validCnt*1.0);
        }                
    }
    return outColor;
}

void main(void)
{
    fragColor = projectTexture();
}