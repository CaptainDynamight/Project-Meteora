#pragma once
#include <GL\glew.h>
#include "RawModel.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "ShaderProgram.h"
#include "Entity.h"
#include "Maths.h"
#include "glm\glm.hpp"
#include <vector>
#include <map>

struct tmCompare
{
public:
	bool operator() (const TexturedModel& t1, const TexturedModel& t2) const
	{
		return ((t1.getRawModel().getVaoID() < t2.getRawModel().getVaoID()) ||
			(t1.getTexture().getID() < t2.getTexture().getID()));
	}
};

typedef std::map<TexturedModel, std::vector<Entity>, tmCompare> tMap;

class EntityRenderer
{
	private:
		const float FOV = 70;
		const float NEAR_PLANE = 0.1f;
		const float FAR_PLANE = 1000.0f;
		int SCR_WIDTH = 700, 
			SCR_HEIGHT = 700;
		glm::mat4 projectionMatrix;
		ShaderProgram shader;

		

	public:
		EntityRenderer() = default;

		EntityRenderer(ShaderProgram shader, glm::mat4 projectionMatrix) : shader(shader)
		{
			shader.start();
			shader.loadProjectionMatrix(projectionMatrix);
			shader.stop();
		}
		
		void render(std::map<TexturedModel, std::vector<Entity>, tmCompare>& entities)
		{
			for (auto& key : entities)
			{
				prepareTexturedModel(key.first);
				for (Entity entity : key.second)
				{
					prepareInstance(entity);
					glDrawElements(GL_TRIANGLES, entity.getModel().getRawModel().getVertexCount(), GL_UNSIGNED_INT, 0);
				}
				unbindTexturedModel();
			}
		}


		void prepareTexturedModel(TexturedModel texturedModel)
		{
			RawModel rawModel = texturedModel.getRawModel();
			glBindVertexArray(rawModel.getVaoID());
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			ModelTexture texture = texturedModel.getTexture();
			if (texture.getHasTransparency())
			{
				glDisable(GL_CULL_FACE);
			}
			shader.loadFakeLightingVariable(texture.getUseFakeLighting());
			shader.loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturedModel.getTexture().getID());
		}


		void unbindTexturedModel()
		{
			glEnable(GL_CULL_FACE);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindVertexArray(0);
		}


		void prepareInstance(Entity& entity)
		{
			glm::mat4 transformationMatrix = Maths::createTransformationMatrix(entity.getPosition(), entity.getRotation(), entity.getScale());
			shader.loadTransformationMatrix(transformationMatrix);
		}

};