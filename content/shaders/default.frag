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

void main() {
	outputColour = vec4(fragmentColour, 1.0f);
}
