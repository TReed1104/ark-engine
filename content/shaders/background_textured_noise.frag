#version 330
// Generic In variables for a fragment shader (mesh colour and TexCoords)
in vec3 fragmentColour;
in vec2 UV;
in vec3 normal;

// Generic Out variables for a fragment shader
out vec4 outputColour;

// Universal uniforms, these match shadertoys
uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;

// Texturing uniforms
uniform bool u_hasTexture;
uniform sampler2D u_textureSampler;

float random(vec2 p) {
	// Pseudo random number generator from seed
	const vec2 r = vec2(23.1406926327792690, 2.6651441426902251);// e^pi (Gelfond's constant) & 2^sqrt(2) (Gelfond-Schneider constant)
	return fract(cos(mod(123456789.0f, 1e-7+256.0f * dot(p, r))));
}

float noise(in vec2 st){
	// taken from examples online
	vec2 i = floor(st);
	vec2 f = fract(st);
	
	// Four corners in 2D of a tile
	float a = random(i);
	float b = random(i + vec2(1.0f, 0.0f));
	float c = random(i + vec2(0.0f, 1.0f));
	float d = random(i + vec2(1.0f, 1.0f));
	
	// Smooth Interpolation
	vec2 u = f * f * (3.0f - 2.0f * f);// Cubic Hermine Curve.
	
	return mix(a, b, u.x) + (c - a) * u.y * (1.0f - u.x) + (d - b) * u.x * u.y;
}

void main() {
	float clampedNosie = mod(noise(gl_FragCoord.xy) * (iTime), 1);
	if(u_hasTexture) {
		outputColour = texture2D(u_textureSampler, UV) * vec4(vec3(clampedNosie), 1.0f);
	}
	else{
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(vec3(clampedNosie), 1.0f);
	}
}
