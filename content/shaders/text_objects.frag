#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform float u_time;
uniform bool u_hasTexture;
uniform sampler2D u_textureSampler;
uniform vec3 u_textColour;

void main() {
	if (u_hasTexture) {
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_textureSampler, UV).r);
    	outputColour = vec4(u_textColour, 1.0) * sampled;
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
