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

// Texturing uniforms
uniform bool u_hasTexture;
uniform sampler2D u_textureSampler;

// 2D Light Struct
struct Light2D {
    vec3 position;
    vec3 colour;
    float radius;
};

uniform Light2D light;

void main() {
    // Calculate the distance to the light
    vec3 distanceFromLight = (light.position - fragmentPosition);
    
    // Calculate how bright the colour on the surface is depending on its surface normal in comparison to the light
    float surfaceBrightness = clamp(dot(normalize(distanceFromLight), fragmentNormal), 0.0, 1.0);
    
    // Reduce the intensity of the light on the surface dependant on its distance to the light
    float lightIntensity = surfaceBrightness * clamp(1.0 - (length(distanceFromLight) / light.radius), 0.0, 1.0);
    
    // Adjust the colour of the texture using the light colour and the calculated intensity
    vec3 pixelColour = light.colour * texture(u_textureSampler, fragmentUV).rgb * lightIntensity;
    
    // Set the output colour of the fragment
    outputColour = vec4(pixelColour, texture(u_textureSampler, fragmentUV).a);
}