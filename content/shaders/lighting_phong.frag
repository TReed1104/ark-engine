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

// Lighting uniforms
uniform vec3 lightingColor;
uniform vec3 lightingPosition;
uniform float ambientStrength;      // How strong is the ambient lighting?
uniform float specularStrength;     // How intense if the specular lighting?

void main() {
    // Ambient calculations
    vec3 ambient = (ambientStrength * lightingColor);
    
    // Diffuse calculations
    vec3 lightDirection = normalize(lightingPosition - fragmentPosition);
    vec3 diffuse = max(dot(normalize(fragmentNormal), lightDirection), 0.0) * lightingColor;

    // Specular calculations
    vec3 viewDirection = normalize(iCameraPosition - fragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, fragmentNormal);
    float shininess = 32;
    vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), shininess) * lightingColor;  

    // Calculate the fragment colour using the vertex colour, ambient, diffuse and specular lighting values
    outputColour = vec4(((ambient + diffuse + specular) * fragmentColour), 1.0);
}