#version 450 compatibility
#ifdef GL_ES
    precision highp float;
#endif
uniform sampler2DArray depthMap;
uniform sampler2DArray colorMap;
uniform sampler2D mainTexture;
uniform int mapSize;
in vec2 texCoord;
in vec4 lightSpacePos[1];
uniform bool _EnableProjection[1];
out vec4 fragColor;

bool flag[1];

vec4 projectTexture() {
    vec4 outColor = vec4(0);
    float bias = 0.005;
    int validCnt = 0;
    for(int i = 0;i<mapSize;i++){
        flag[i] = false;
    }
    for(int i = 0;i<mapSize;i++){
        vec3 projCoords = lightSpacePos[i].xyz;
        if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1)
            continue;
        float closestDepth = texture(depthMap, vec3(projCoords.xy, i)).r;
        float currentDepth = projCoords.z;
        flag[i] = currentDepth < closestDepth + 0.005f;
        validCnt += flag[i]? 1 : 0;                
    }
    if(validCnt == 0)
        return outColor;
    for(int i = 0;i<mapSize;i++){
        if(flag[i]&&_EnableProjection[i]){
            vec3 projCoords = lightSpacePos[i].xyz;
            outColor += texture(colorMap, vec3(projCoords.xy, i)) / (validCnt*1.0);
        }                
    }
    return outColor;
}

void main()
{
    vec4 col = texture(mainTexture, texCoord);
    vec4 projColor = projectTexture();
    fragColor = projColor.x == 0?col/2:projColor;
    // fragColor = col/2 + projectTexture()/2;
    fragColor.a = 1.0;
}