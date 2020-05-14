#pragma once
#include "RawModel.h"
#include "ModelTexture.h"

class TexturedModel
{
	private:
		RawModel rawModel;
		ModelTexture texture;

	public:
		TexturedModel(RawModel model, ModelTexture texture) : rawModel(model), texture(texture)
		{
			this->rawModel = model;
			this->texture = texture;
		}

		RawModel getRawModel() const
		{
			return rawModel;
		}
		ModelTexture getTexture() const
		{
			return texture;
		}
};