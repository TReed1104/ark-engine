#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform sampler2DArray textureSampler;

void main() {
	// Set the colour to pass to the rasteriser.
	outputColour = texture(textureSampler, vec3(UV, 0));
}
