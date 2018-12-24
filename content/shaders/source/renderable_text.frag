#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform bool hasTexture;
uniform sampler2D textureSampler;
uniform vec3 textColour;

void main() {
	if (hasTexture) {
		// Set the colour to pass to the rasteriser.
		//outputColour = texture2D(textureSampler, UV);
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textureSampler, UV).r);
    	outputColour = vec4(textColour, 1.0) * sampled;
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
