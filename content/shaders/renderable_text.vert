#version 330
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexUV;

out vec3 fragmentColour;
out vec2 UV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
	fragmentColour = vertexColor;
	vec4 newPosition = vec4(vertexPosition, 1.0);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * newPosition;
	UV = vertexUV;
}
