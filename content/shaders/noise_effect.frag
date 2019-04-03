#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform float time;

float random(vec2 p){
	// Pseudo random
	const vec2 r=vec2(23.1406926327792690,2.6651441426902251);// e^pi (Gelfond's constant) & 2^sqrt(2) (Gelfond–Schneider constant)
	return fract(cos(mod(123456789.f,1e-7+256.f*dot(p,r))));
}
float noise(in vec2 st){
	// taken from examples online
	vec2 i=floor(st);
	vec2 f=fract(st);
	
	// Four corners in 2D of a tile
	float a=random(i);
	float b=random(i+vec2(1.f,0.f));
	float c=random(i+vec2(0.f,1.f));
	float d=random(i+vec2(1.f,1.f));
	
	// Smooth Interpolation
	vec2 u=f*f*(3.f-2.f*f);// Cubic Hermine Curve.
	
	return mix(a,b,u.x)+(c-a)*u.y*(1.f-u.x)+(d-b)*u.x*u.y;
}
void main(){
	vec2 st=gl_FragCoord.xy;
	
	// Scale the coordinate system to see
	// some noise in action
	vec2 pos=vec2(st);
	
	// Use the noise function
	float n=noise(pos)*(time);
	
	// clamp the noise to a value between 0 and 1 for use in the fragment colour
	float clampedNosie=mod(n,1);
	outputColour=vec4(vec3(clampedNosie),1.f);
}
