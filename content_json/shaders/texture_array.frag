#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform bool hasTexture;
uniform sampler2DArray textureSampler;
uniform int textureArrayLayer;

void main() {
	if (hasTexture) {
		// Set the colour to pass to the rasteriser.
		outputColour = texture(textureSampler, vec3(UV, textureArrayLayer));
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
