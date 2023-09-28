#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D _MainTex;

uniform vec4 _BackColor;
uniform float _FlashFrequency;
uniform float _OuterWidth;
uniform float _PatternDensity;
uniform float _PatternWidth;
uniform vec4 _PatternColor;
uniform int _PatternShape;
uniform float _AnimSpeed;
uniform float _PathWidth;
uniform float _OuterGradientLowerBound;
uniform vec4 _OuterColor;
uniform float osg_FrameTime;
vec2 plotTexture(vec2 uv, float time)
{
    uv.y = (uv.y + time * _AnimSpeed) * _PatternDensity;
    vec2 f_uv = uv;
    vec4 texColor = texture(_MainTex, f_uv.yx);
    vec2 pattern = vec2(texColor.a, 1.0 - texColor.a);
    return pattern;
}

vec2 plotLine(vec2 uv, float time)
{
    uv.y = uv.y + time * _AnimSpeed;
    vec2 f_uv = fract(uv * _PatternDensity);
    vec2 pattern = vec2(step(1.0 - _PatternWidth, f_uv.y), 1.0);
    return pattern;
}

vec2 plotArrow(vec2 uv, float time)
{
    float angle = time * 5.0;
    mat2 rotationMatrix = mat2(
        cos(radians(angle)), sin(radians(angle)),
        -sin(radians(angle)), cos(radians(angle))
    );
    uv.y = (uv.y + time * _AnimSpeed) * _PatternDensity;
    // uv = rotationMatrix * uv;
    vec2 f_uv = fract(uv);
    float dist = uv.x > 0.5 ? abs(f_uv.y - f_uv.x * 2.0 + 0.75) : abs(f_uv.y + f_uv.x * 2.0 - 1.25);
    vec2 pattern = vec2(step(1.0 - _PatternWidth, 1.0 - dist), 1.0);
    return pattern;
}

vec2 plotPattern(vec2 uv, float time)
{
    vec2 pattern = vec2(1.0, 1.0);
    if (_PatternShape == 0) {
        pattern = plotTexture(uv, time);
    }
    else if (_PatternShape == 1) {
        pattern = plotLine(uv, time);
    }
    else if (_PatternShape == 2) {
        pattern = plotArrow(uv, time);
    }
    return pattern;
}

void main()
{
    if(_OuterWidth==1){
        FragColor = mix(_OuterColor* _OuterGradientLowerBound,_OuterColor ,abs(0.5-TexCoords.x)*2);
        return;
    }
    vec2 uv = vec2((TexCoords.x - _OuterWidth * 0.5) / (1.0 - _OuterWidth), TexCoords.y);
    vec2 pattern = (uv.x >= 0.0 && uv.x <= 1.0) ? plotPattern(uv, osg_FrameTime) : vec2(0.0, 0.0);
    float outerWidthScale = 0.5*_OuterWidth/(1-_OuterWidth);
    vec4 backColor = (uv.x >= 0.0&&uv.x<=1.0) ?(1.0 - pattern.x) * _BackColor:vec4(0,0,0,0);
    vec4 patternColor = _PatternColor * pattern.x * pattern.y;
    vec4 outerColor = vec4(0.0, 0.0, 0.0, 0.0);
    float t = (uv.x + outerWidthScale) / outerWidthScale;
    outerColor = (uv.x < 0.0) ? mix(_OuterColor, _OuterColor * _OuterGradientLowerBound, t) : outerColor;
    t = (uv.x - 1.0) / outerWidthScale;
    outerColor = (uv.x > 1.0) ? mix(_OuterColor, _OuterColor * _OuterGradientLowerBound, 1-t) : outerColor;

    float brightness = sin(osg_FrameTime * _FlashFrequency) + 1.5;
    patternColor.rgb *= brightness;

    vec4 fragColor = clamp(backColor + patternColor + outerColor, 0.0, 1.0);
    FragColor = fragColor;
}
