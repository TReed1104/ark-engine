#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform bool hasTexture;
uniform sampler2D textureSampler;
uniform vec2 frameBorderSize;
uniform vec2 textureDimensions;
uniform vec2 sourceFrameSize;
uniform vec2 sourceFramePosition;

void main() {
	if (hasTexture) {
        vec2 pixelSizeInUVs = vec2(1, 1) / textureDimensions;
        vec2 borderedFrameSize = (sourceFrameSize + (frameBorderSize * 2));
        vec2 borderedFrameSizeInUV = pixelSizeInUVs * borderedFrameSize;
        vec2 borderedFramePositionInUV = borderedFrameSizeInUV * sourceFramePosition;

        vec2 offSetFramePositionInUV = borderedFramePositionInUV + (pixelSizeInUVs * frameBorderSize);
        vec2 newUV = (offSetFramePositionInUV + (UV / ((textureDimensions / sourceFrameSize))));
		outputColour = texture2D(textureSampler, newUV);
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
