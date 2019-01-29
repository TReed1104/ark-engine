#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

void main() {
	outputColour = vec4(fragmentColour, 1.0f);
}
