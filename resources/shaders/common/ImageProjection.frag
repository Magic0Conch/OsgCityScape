#version 450 compatibility
#ifdef GL_ES
    precision highp float;
#endif
uniform sampler2D depthMap;
uniform sampler2D colorMap;
uniform sampler2D mainTexture;
uniform int mapSize;
in vec2 texCoord;
in vec4 lightSpacePos;
uniform bool _EnableProjection;
out vec4 fragColor;
uniform vec2 u_ScreenSize; 
bool flag;

vec4 projectTexture() {
    //screen Texture, screen uv
    vec2 uv = gl_FragCoord.xy / u_ScreenSize;
    vec4 outColor = texture(mainTexture, uv);
    float bias = 0.005;
    int validCnt = 0;
    flag = false;

    
    vec3 projCoords = lightSpacePos.xyz;
    if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1)
        return outColor;
    float closestDepth = texture(depthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    flag = currentDepth < closestDepth + 0.005f;
    
    
    if(!flag)
        return outColor;

    outColor = texture(colorMap, projCoords.xy);
    return outColor;
}

void main()
{
	// FragColor = texture(texture0,TexCoord0);
	// if(FragColor.a <= 0.9)
	// {
	// 	FragColor.a = 0;
	// 	return;
	// }
	// float sampler_x = (gl_FragCoord[0])/width;
	// float sampler_y = (gl_FragCoord[1])/height;
	// vec2 sampler0 = vec2(sampler_x, sampler_y);
	// vec2 sampler1 = vec2(sampler0[0]-1.0/width,sampler0[1]);
	// vec2 sampler2 = vec2(sampler0[0]+1.0/width,sampler0[1]);
	// vec2 sampler3 = vec2(sampler0[0],sampler0[1]-1.0/height);
	// vec2 sampler4 = vec2(sampler0[0],sampler0[1]+1.0/height);

	// FragColor.a = (texture(texture1,sampler0).r
	// 			+ texture(texture1,sampler1).r 
	// 			+ texture(texture1,sampler2).r
	// 			+ texture(texture1,sampler3).r
	// 			+ texture(texture1,sampler4).r)/5.0;
	// fix holes and cut off edges(0.1% on each side).
	// if(TexCoord0[0]>=1+2.0/height||TexCoord0[0]<=-2.0/height||TexCoord0[1]>=1+2.0/height||TexCoord0[1]<=-2.0/height || FragColor.a <= 0.2)
	// {
	// 	FragColor.a = 0;
	// }
	// else
	// { 
	// 	FragColor.a = 1;
	// }
    // fragColor = projectTexture();
    fragColor = vec4(1,0,0,1);
    // fragColor.a = 1.0;
    // fragColor.r = 1.0;
}