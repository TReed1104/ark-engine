#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

// Universal uniforms
uniform float u_time;
uniform vec2 u_resolution;

// Universal uniforms
uniform bool u_hasTexture;
uniform sampler2D u_textureSampler;
uniform vec3 u_textColour;

void main() {
	if (u_hasTexture) {
    	outputColour = vec4(u_textColour, 1.0) * vec4(1.0, 1.0, 1.0, texture(u_textureSampler, UV).r);
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
