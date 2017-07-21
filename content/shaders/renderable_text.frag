#version 330
in vec3 fragmentColour;

out vec4 outputColour;

uniform bool hasTexture;
uniform sampler2D textureSampler;

void main() {
	outputColour = vec4(fragmentColour, 1.0f);
}
