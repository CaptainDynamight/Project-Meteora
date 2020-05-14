#pragma once

class TerrainTexture
{
	private:
		unsigned int textureID;

	public:
		TerrainTexture() = default;

		TerrainTexture(unsigned int textureID)
		{
			this->textureID = textureID;
		}

		unsigned int getTextureID()
		{
			return textureID;
		}
};