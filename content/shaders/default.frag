#version 330
in vec3 fragmentColour;

out vec4 outputColour;

// Universal uniforms
uniform float u_time;
uniform vec2 u_resolution;

void main() {
	outputColour = vec4(fragmentColour, 1.0f);
}
