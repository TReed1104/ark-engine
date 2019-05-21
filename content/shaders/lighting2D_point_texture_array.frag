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
uniform sampler2DArray u_textureSampler;
uniform int u_textureArrayLayer;

// 2D Light Struct
struct Light2D {
    vec3 position;
    vec2 direction;     // used for spotlighting

    vec3 colour;
    float ambientIntensity;
    float attenuation;    // How far can the light reach?
    
    float spotlightAngle;
    float spotlightCutOff;
};

uniform Light2D light;


void main() {
    vec3 ambient = (light.colour * light.ambientIntensity) * texture(u_textureSampler, vec3(fragmentUV, u_textureArrayLayer)).rgb;
    vec3 diffuse = light.colour * texture(u_textureSampler, vec3(fragmentUV, u_textureArrayLayer)).rgb;

    outputColour = vec4((ambient + diffuse), texture(u_textureSampler, vec3(fragmentUV, u_textureArrayLayer)).a);

}