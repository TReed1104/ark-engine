#version 330
// Generic In variables for a vertex shader
in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexUV;
in vec3 vertexNormal;

// Generic Out variables for a vertex shader, these are the values passed to the fragment shader
out vec3 fragmentPosition;
out vec3 fragmentColour;
out vec2 fragmentUV;
out vec3 fragmentNormal;

// Model, View, Projection uniforms -> used for deciding how to render a surface in 3D space
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main() {
	// Set our vertex output values, these are what we use inside the fragment shaders
	fragmentPosition = vec3(u_modelMatrix * vec4(vertexPosition, 1.0));
	fragmentColour = vertexColor;
	fragmentUV = vertexUV;
	fragmentNormal = mat3(transpose(inverse(u_modelMatrix))) * vertexNormal;

	// Output the positional value after applying the transformation matrices
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(vertexPosition, 1.0);
}
