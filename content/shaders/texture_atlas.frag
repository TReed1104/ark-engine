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
		// Calculate the size of a single pixel of the texture in UVs (0.0-1.0)
		vec2 pixelSizeInUVs = vec2(1, 1) / textureDimensions;
		vec2 halfPixelSizeInUVs = (pixelSizeInUVs / 2);		// Size of half a pixel, this was a test for offsetting texels to investigate the texture bleeding

		// Calculate the size of each tile including the border
		vec2 borderedFrameSize = (sourceFrameSize + (frameBorderSize * 2));
		// Calculate the size of the tile and its border in UVs
		vec2 borderedFrameSizeInUV = pixelSizeInUVs * borderedFrameSize;

		// Calculate the position of the frame we want on the texture in UVs
		vec2 borderedFramePositionInUV = borderedFrameSizeInUV * sourceFramePosition;
		// Calculate the offset from the above position that the actual texture we want is, ignoring the border
		vec2 offSetFramePositionInUV = borderedFramePositionInUV + (pixelSizeInUVs * frameBorderSize);

		// Use all the above math and the UV passed from the vertex shader to calculate the part of the texture we actually want to render.
		vec2 newUV = offSetFramePositionInUV + (UV / ((textureDimensions / sourceFrameSize)));

		// Set the colour of the pixel to pass to the rasteriser.
		outputColour = texture2D(textureSampler, newUV);
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
