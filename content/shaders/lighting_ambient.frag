#version 330
// Generic In variables for a fragment shader (mesh colour and TexCoords)
in vec3 fragmentColour;
in vec2 UV;
in vec3 normal;

// Generic Out variables for a fragment shader
out vec4 outputColour;

// Universal uniforms, these match shadertoys
uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;

// Lighting uniforms
uniform vec3 lightingColor;
uniform vec3 lightingPosition
uniform float ambientStrength;  // How strong is the ambient lighting?

void main() {
    // Ambient calculations
    vec3 ambient = (ambientStrength * lightingColor);

    // Calculate the fragment colour using the vertex colour and ambient lighting values
    outputColour = vec4((ambient * fragmentColour), 1.0);
}