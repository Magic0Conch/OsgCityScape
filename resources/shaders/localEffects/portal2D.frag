#version 330 core


//lib function begin
#define NOISE_SIMPLEX_1_DIV_289 0.00346020761245674740484429065744f
vec2 mod289(vec2 x)
{
	return x - floor(x * NOISE_SIMPLEX_1_DIV_289) * 289.0;
}

vec3 mod289(vec3 x)
{
	return x - floor(x * NOISE_SIMPLEX_1_DIV_289) * 289.0;
}

vec3 taylorInvSqrt(vec3 r)
{
	return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 permute(vec3 x)
{
	return mod289(x * x * 34.0 + x);
}

float snoise(vec2 v)
{
	const vec4 C = vec4(0.211324865405187, // (3.0-sqrt(3.0))/6.0
	0.366025403784439, // 0.5*(sqrt(3.0)-1.0)
	- 0.577350269189626, // -1.0 + 2.0 * C.x
	0.024390243902439); // 1.0 / 41.0

	// First corner
	vec2 i = floor(v + dot(v, C.yy));
	vec2 x0 = v - i + dot(i, C.xx);
	
	// Other corners
	vec2 i1;
	i1.x = step(x0.y, x0.x);
	i1.y = 1.0 - i1.x;
	
	// x1 = x0 - i1  + 1.0 * C.xx;
	// x2 = x0 - 1.0 + 2.0 * C.xx;
	vec2 x1 = x0 + C.xx - i1;
	vec2 x2 = x0 + C.zz;
	
	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0))
	+ i.x + vec3(0.0, i1.x, 1.0));
	
	vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x1, x1), dot(x2, x2)), 0.0);
	m = m * m;
	m = m * m;
	
	// Gradients: 41 points uniformly over a line, mapped onto a diamond.
	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;
	
	// Normalise gradients implicitly by scaling m
	m *= taylorInvSqrt(a0 * a0 + h * h);
	
	// Compute final noise value at P
	vec3 g;
	g.x = a0.x * x0.x + h.x * x0.y;
	g.y = a0.y * x1.x + h.y * x1.y;
	g.z = a0.z * x2.x + h.z * x2.y;
	return 130.0 * dot(m, g);
}
//lib function end


out vec4 FragColor;
in vec2 TexCoords;

//parameters
uniform sampler2D _MainTex;
uniform sampler2D _OriginalTexture;
uniform float osg_FrameTime;

uniform float _EmitCenterUVX;
uniform float _EmitCenterUVY;
uniform float _EffectOutterHeight;
uniform float _EffectOutterWidth;
uniform float _EffectInnerWidth;        

uniform vec4 _WaveColor;
uniform float _Amplitude;
uniform float _Frequency;
uniform float _WaveWidth;
uniform float _WaveSpeed;

uniform float _SeamWidth;
uniform bool _IsHorizontal;


vec4 portal(vec2 uv){
    float side = smoothstep(0.5, 0.2, abs(uv.x));
    float center = smoothstep(_SeamWidth, 0.0, length(uv.x));
    vec2 rd = vec2(uv.x,uv.y*_Amplitude);
    float time = 15.0f+osg_FrameTime;
    float t = (time+1.)*(0.5-abs(uv.x));
    float n2 = snoise((rd*t+t)*_Frequency/abs(uv.x*t)+(time)*_WaveSpeed);
    float flare = smoothstep(.0,1.,_WaveWidth*.1/length(abs(uv.x)*n2))*side;
    flare = flare-center*clamp((t-1.5)*1.,0.,1.);
    vec3 col = _WaveColor.rgb*2.;
    col *= flare;
    return vec4(col,1.0*flare*_WaveColor.a);
}

void main(){
    vec2 uv = TexCoords;
    vec4 originalColor = texture2D(_OriginalTexture,uv);
    uv = _IsHorizontal?vec2(uv.y,uv.x):uv;
    vec4 blurColor = texture2D(_MainTex,uv);

    float effectRatio = smoothstep(_EffectOutterWidth/2.0,_EffectInnerWidth/2.0,abs(uv.x-_EmitCenterUVX));
    effectRatio*=smoothstep(_EffectOutterHeight/2.0,_EffectOutterHeight/4.0,abs(uv.y-_EmitCenterUVY));
    uv-=.5;
    uv.x = uv.x - _EmitCenterUVX+.5;
    uv.x/=_EffectOutterWidth;
    uv.y = uv.y - _EmitCenterUVY+.5;
    uv.y/=_EffectOutterHeight;
    effectRatio *= uv.y>=-0.5&&uv.y<=0.5?1.:0.;
    vec4 waveColor = uv.y>=-0.5&&uv.y<=0.5? portal(uv):vec4(.0f);
    vec4 additiveColor = mix(blurColor,waveColor,waveColor.a);
    FragColor = mix(originalColor,additiveColor,effectRatio);
	//gamma correction
    float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}