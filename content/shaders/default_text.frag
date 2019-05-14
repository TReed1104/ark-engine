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
uniform vec3 iCameraPosition;

// Texturing uniforms
uniform bool u_hasTexture;
uniform sampler2D u_textureSampler;
uniform vec3 u_textColour;

void main() {
	if (u_hasTexture) {
    	outputColour = vec4(u_textColour, 1.0) * vec4(1.0, 1.0, 1.0, texture(u_textureSampler, fragmentUV).r);
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
