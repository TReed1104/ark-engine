#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform bool hasTexture;
uniform sampler2D textureSampler;
uniform vec2 textureDimensions;
uniform vec2 sourceFrameSize;
uniform vec2 sourceFramePosition;

void main() {
	if (hasTexture) {
		// Texturing has been setup, use the texture.
		// Calculate the delta of each frame.
		float uvPosX = (1 / (textureDimensions.x / sourceFrameSize.x)) * sourceFramePosition.x;
		float uvPosY = (1 / (textureDimensions.y / sourceFrameSize.y)) * sourceFramePosition.y;
		// Calculate the new position of the UV.
		float frameX = uvPosX + (UV.x / (textureDimensions.x / sourceFrameSize.x));
		float frameY = uvPosY + (UV.y / (textureDimensions.y / sourceFrameSize.y));
		// Set the new UVs with the values.
		vec2 newUV = vec2(frameX, frameY);

		// Set the colour of the pixel we are drawing.
		outputColour = texture2D(textureSampler, newUV);
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
