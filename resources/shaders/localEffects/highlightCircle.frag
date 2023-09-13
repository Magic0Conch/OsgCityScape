#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform lowp vec4 _InnerColor;
uniform lowp vec4 _PatternColor;
uniform lowp vec4 _OutlineColor;
uniform mediump float _FlashFrequency;
uniform mediump float _PatternDensity;
uniform mediump float _PatternWidth;
uniform mediump int _PatternShape;
uniform mediump float _AnimSpeed;
uniform float osg_FrameTime;
uniform lowp float _OutlineRatio;

#define PI 3.1415926535
in VS_OUT{
    lowp vec2 uv;
    mediump vec3 localPos;
}vs_out;


float plotGrid(vec2 uv,float width){
    vec2 uv_anim = vec2(mix(0.0,1.0,uv.x+width/2/_PatternDensity + osg_FrameTime * _AnimSpeed), mix(0.0,1.0,uv.y+ width/2/_PatternDensity + osg_FrameTime*_AnimSpeed));    vec2 f_uv = vec2(fract(uv_anim.x*_PatternDensity),fract(uv_anim.y*_PatternDensity));
    float top = step(width,f_uv.x);
    float right = step(width,f_uv.y);
    return 1-step(0.5,top*right);
}

float plotCircle(vec2 uv){
    float uv_anim = mix(0.0,1.0,uv.x + osg_FrameTime * _AnimSpeed);
    float f_uv = fract(uv_anim*_PatternDensity);
    return step(1-_PatternWidth,f_uv);
}

float plotDot(vec2 uv,float width,float p_density){
    vec2 uv_anim = vec2(mix(0.0,1.0,uv.x + osg_FrameTime * _AnimSpeed), mix(0.0,1.0,uv.y + osg_FrameTime * _AnimSpeed));
    vec2 f_uv = fract(uv_anim*p_density);
    f_uv.x = f_uv.x<0.5?f_uv.x:1-f_uv.x;
    f_uv.y = f_uv.y<0.5?f_uv.y:1-f_uv.y;
    float r = (width*0.5)*(width*0.5);
    return 1-step(r,f_uv.x*f_uv.x + f_uv.y*f_uv.y);

}

float plotRadialLine(vec2 uv,float angle){
    vec2 l = vec2(cos(angle),sin(angle));
    float d = dot(uv,l)/length(l);
    float h = length(uv-l*d);
    return 1-step(_PatternWidth/10,h);
}

float plotRadar(vec2 xy,vec2 uv){
    //lowp vec2 uv_anim = lowp vec2(lerp(0.0,1.0,uv.x + osg_FrameTime * _AnimSpeed), lerp(0.0,1.0,uv.y + osg_FrameTime * _AnimSpeed));
    //lowp vec2 uv_anim = lowp vec2(uv.x + lerp(0.0,1.0, osg_FrameTime * _AnimSpeed), uv.y + lerp(0.0,1.0,osg_FrameTime * _AnimSpeed));
    float acc = 0.0;
    for(float i = 0;i<_PatternDensity;i++){
        float t = i/_PatternDensity*2;
        float angle = t*PI;
        acc+=plotRadialLine(xy,angle);
    }
    acc+=plotCircle(uv);
    return step(0.5,acc);                
}

float plotLine(vec2 uv){
    vec2 uv_anim = vec2(mix(0.0,1.0,uv.x + osg_FrameTime * _AnimSpeed),mix(0.0,1.0,uv.y + osg_FrameTime * _AnimSpeed));
    vec2 f_uv = fract(uv_anim*_PatternDensity);                
    float right = step(1-_PatternWidth,f_uv.y);
    return step(0.5,right);
}

float plotGridAndDot(vec2 uv){
    float acc = 0.0;
    acc+=plotGrid(uv,_PatternWidth);
    acc+=plotDot(uv,_PatternWidth*3,_PatternDensity);
    acc+=plotDot(uv,_PatternWidth*5,_PatternDensity*6);
    return step(0.5,acc);
}

void main(){
    vec2 localPosXZ = vs_out.localPos.xz/(1-_OutlineRatio);
    vec2 uv = vs_out.uv/(1-_OutlineRatio);
    if(uv.x<=1){
        float pattern;
        if(_PatternShape==0){
            pattern=0;
        }
        else if(_PatternShape==1){
            pattern = plotGrid(localPosXZ,_PatternWidth);
        }
        else if(_PatternShape==2){
            pattern = plotDot(localPosXZ,_PatternWidth,_PatternDensity);                
        }
        else if(_PatternShape==3){
            pattern = plotGrid(uv,_PatternWidth);                                
        }
        else if(_PatternShape==4){
            pattern = plotRadar(localPosXZ,uv);
        }
        else if(_PatternShape==5){
            pattern = plotLine(localPosXZ);
        }
        else if(_PatternShape==6){
            pattern = plotGridAndDot(localPosXZ);
        }
        vec4 backColor = vec4((1-pattern) * _InnerColor.rgb,(1-pattern)* _InnerColor.a);
        vec4 patternColor = vec4(pattern * _PatternColor.rgb,pattern * _PatternColor.a);
        float brightness = (sin(osg_FrameTime* _FlashFrequency*2)+1.5)/2;
        patternColor.rgb*=brightness;
        FragColor = backColor + patternColor;
    }
    else{
        FragColor = _OutlineColor;
    }
    float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}