#pragma once
#include "TerrainShader.h"
#include "Terrain.h"
#include <vector>
#include "TexturedModel.h"
#include "Entity.h"

class TerrainRenderer
{
	private:
		TerrainShader shader;

	public:
		TerrainRenderer() = default;

		TerrainRenderer(TerrainShader tshader, glm::mat4 projectionMatrix) : shader(tshader)
		{
			shader.start();
			shader.loadProjectionMatrix(projectionMatrix);
			shader.connectTextureUnits();
			shader.stop();
		}

		void render(std::vector<Terrain> terrains)
		{
			for (Terrain terrain : terrains)
			{
				prepareTerrain(terrain);
				loadModelMatrix(terrain);
				glDrawElements(GL_TRIANGLES, terrain.getModel().getVertexCount(), GL_UNSIGNED_INT, 0);
				unbindTexturedModel();
			}
		}

		void prepareTerrain(Terrain terrain)
		{
			RawModel rawModel = terrain.getModel();
			glBindVertexArray(rawModel.getVaoID());
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			bindTextures(terrain);
			shader.loadShineVariables(1, 0);
			
		}

		void bindTextures(Terrain terrain)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, terrain.getTexturePack().getBackgroundTexture().getTextureID());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, terrain.getTexturePack().getRTexture().getTextureID());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, terrain.getTexturePack().getGTexture().getTextureID());
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, terrain.getTexturePack().getBTexture().getTextureID());
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, terrain.getBlendMap().getTextureID());
		}

		void unbindTexturedModel()
		{
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindVertexArray(0);
		}


		void loadModelMatrix(Terrain terrain)
		{
			glm::mat4 transformationMatrix = Maths::createTransformationMatrix(glm::vec3(terrain.getX(), 0, terrain.getZ()), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
			shader.loadTransformationMatrix(transformationMatrix);
		}
};