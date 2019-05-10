#version 330
// Generic In variables for a fragment shader
in vec3 fragmentPosition;
in vec3 fragmentColour;
in vec2 fragmentUV;
in vec3 fragmentNormal;

// Generic Out variables for a fragment shader
out vec4 outputColour;

// Universal uniforms, these match shadertoys
uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;

// Texturing uniforms
uniform bool u_hasTexture;
uniform sampler2DArray u_textureSampler;
uniform int u_textureArrayLayer;

void main() {
	if (u_hasTexture) {
		outputColour = texture(u_textureSampler, vec3(fragmentUV, u_textureArrayLayer));
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
