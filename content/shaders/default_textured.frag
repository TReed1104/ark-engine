#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

// Universal uniforms
uniform float u_time;
uniform vec2 u_resolution;

// Shader specific uniforms
uniform bool u_hasTexture;
uniform sampler2D u_textureSampler;

void main() {
	if (u_hasTexture) {
		outputColour = texture2D(u_textureSampler, UV);
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
