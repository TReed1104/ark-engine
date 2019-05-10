#version 330
// Generic In variables for a vertex shader, these are normally values from an object mesh
in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexUV;
in vec3 vertexNormal;

// Generic Out variables for a vertex shader, these are the values passed to the fragment shader
out vec3 fragmentColour;
out vec2 UV;
out vec3 normal;

// Model, View, Projection uniforms -> used for deciding how to render a surface in 3D space
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main() {
	fragmentColour = vertexColor;
	vec4 newPosition = vec4(vertexPosition, 1.0);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * newPosition;
	UV = vertexUV;
}
