#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform lowp vec4 _Color;
uniform lowp vec4 _PatternColor;
uniform mediump float _AnimSpeed;
uniform lowp float _PatternDensity;
uniform lowp float _PatternWidth;

uniform mediump float _FlashFrequency;
uniform mediump float _OuterWidth;
uniform lowp int _PatternShape;
uniform mediump float _Radius;
uniform mediump float _Height;
uniform lowp int _BackStyle;

uniform float osg_FrameTime;

#define PI 3.1415926535
in VS_OUT{
    lowp vec2 uv;
    lowp float alpha;
    mediump vec3 localPos;
}vs_out;

float fmod(float x, float y) {
    return x - y * floor(x / y);
}

float luminance(vec4 col){
    return col.r * 0.2125 + col.g * 0.7154 + col.b * 0.0721;
}

vec2 plotLine(vec2 uv,float speed,float PatternDensity,float PatternWidth){
    vec2 st = vec2(uv.x,mix(0.0,1.0,uv.y+osg_FrameTime*speed));
    vec2 f_st = fract(st*PatternDensity);
    float alpha = step(1 - PatternWidth,f_st.y)>0.5?1.0:0;
    return vec2(step(1 - PatternWidth,f_st.y),alpha);
}

vec2 plotGrid(vec2 uv,float width){
    float angle = 45; 
    mat2 rotationMatrix = mat2(cos(radians(angle)), sin(radians(angle)),
                                        -sin(radians(angle)), cos(radians(angle)));
    vec2 rotate_uv = rotationMatrix*vec2(uv.x,uv.y);
    float alpha = mix(0,_Height,uv.y)/_Height;
    vec2 f_uv = fract(rotate_uv*_PatternDensity);
    float top = step(width,f_uv.x);
    float right = step(width,f_uv.y);
    alpha = 1-step(0.5,top*right)>0.5?alpha:0;
    return vec2(1-step(0.5,top*right),alpha);
}

vec2 plotVerticalLine(vec2 uv){
    float angle = 20; 
    mat2 rotationMatrix = mat2(cos(radians(angle)), sin(radians(angle)),
                                        -sin(radians(angle)), cos(radians(angle)));
    vec2 rotate_uv = rotationMatrix*vec2(uv.x,uv.y);

    vec2 st = vec2(mix(0.0,1.0,rotate_uv.y+osg_FrameTime*_AnimSpeed),rotate_uv.x);
    vec2 f_st = fract(st*_PatternDensity);
    float alpha =  1 - abs(uv.y-_Height/2)/_Height*2;
    alpha = step(1 - _PatternWidth,f_st.y)>0.5?alpha:0;
    return vec2(step(1 - _PatternWidth,f_st.y),alpha);
}

float radialDistance(vec2 uv){
    float angle = fmod(osg_FrameTime*_AnimSpeed,2*PI);
    float fragAngle = atan(uv.y,uv.x)+PI;
    return fmod(angle - fragAngle + 2*PI,2*PI)/(2*PI);
}

void main()
{
    vec2 pattern;
    vec2 xz = vec2(vs_out.localPos.x,vs_out.localPos.z);
    float fragAngle = atan(xz.y,xz.x)+PI;
    float radius = length(vs_out.localPos.xz);
    vec2 uv = vec2(fragAngle*_Radius,vs_out.localPos.y);

    if(_PatternShape==1){
        pattern = plotLine(vs_out.uv,_AnimSpeed,_PatternDensity,_PatternWidth);
    }
    else if(_PatternShape==2){
        pattern = plotGrid(uv,_PatternWidth);
    }
    else if(_PatternShape==3){
        pattern = plotVerticalLine(uv);
    }


    pattern = vs_out.localPos.y/_Height>1-_OuterWidth?vec2(1,1):pattern;

    vec4 backColor = vec4((1-pattern.x) * _Color.rgb,(1-pattern.x)* _Color.a*vs_out.alpha);
    
    if(_BackStyle==1){
        backColor.a *= radialDistance(vs_out.localPos.xz);                
    }
    vec4 patternColor = vec4(pattern.x * _PatternColor);
    patternColor.a = clamp(pattern.y,0,1);
    

    float brightness = sin(osg_FrameTime*_FlashFrequency)+1.5;
    patternColor.rgb *= brightness;

    FragColor = backColor + patternColor;                
    
}