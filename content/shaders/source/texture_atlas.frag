#version 330
in vec3 fragmentColour;
in vec2 UV;

out vec4 outputColour;

uniform bool hasTexture;
uniform sampler2D textureSampler;
uniform ivec2 frameBorderSize;
uniform ivec2 textureDimensions;
uniform ivec2 sourceFrameSize;
uniform ivec2 sourceFramePosition;

void main() {
	if (hasTexture) {
		// Calculate the size of a single pixel of the texture in UVs (0.0-1.0)
		vec2 pixelSizeInUVs = vec2(1, 1) / textureDimensions;
		// Calculate the size of each tile including the border
		ivec2 borderedFrameSize = (sourceFrameSize + (frameBorderSize * 2));
		// Calculate where the frame is to be positioned on the spritesheet
		ivec2 borderedFramePositionInPixels = borderedFrameSize * sourceFramePosition;
		// Convert that position to Texels
		vec2 borderedFramePositionInUV = pixelSizeInUVs * borderedFramePositionInPixels;
		// Calculate the offset from the above position that the actual texture we want is, ignoring the border
		vec2 framePositionInUVs = borderedFramePositionInUV + (pixelSizeInUVs * frameBorderSize);
		// Use all the above math and the UV passed from the vertex shader to calculate the part of the texture we actually want to render.
		vec2 newUV = framePositionInUVs + (UV / ((textureDimensions / (sourceFrameSize - (pixelSizeInUVs * frameBorderSize)))));
		// Set the colour to pass to the rasteriser.
		outputColour = texture2D(textureSampler, newUV);
	}
	else {
		// Texturing has not been setup, use the colour buffer.
		outputColour = vec4(fragmentColour, 1.0f);
	}
}
