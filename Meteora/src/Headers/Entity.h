#pragma once
#include "TexturedModel.h"
#include "glm\glm.hpp"

class Entity
{
	private:
		TexturedModel m_model;
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

	public:
		Entity(TexturedModel model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : m_model(model), m_position(position), m_rotation(rotation), m_scale(scale)
		{
			m_model = model;
			m_position = position;
			m_rotation = rotation;
			m_scale = scale;
		}

		TexturedModel getModel()
		{
			return m_model;
		}
		
		glm::vec3 getPosition()
		{
			return m_position;
		}
		
		glm::vec3 getRotation() 
		{
			return m_rotation;
		}
		
		glm::vec3 getScale()
		{
			return m_scale;
		}

		void setModel(TexturedModel model)
		{
			m_model = model;
		}
		
		void setPosition(glm::vec3 position)
		{
			m_position = position;
		}
		
		void setRotation(glm::vec3 rotation)
		{
			m_rotation = rotation;
		}
		
		void setScale(glm::vec3 scale)
		{
			m_scale = scale;
		}

		void increasePosition(glm::vec3 position)
		{
			m_position += position;
		}

		void increaseRotation(glm::vec3 rotation)
		{
			m_rotation += rotation;
		}

		void increaseScale(glm::vec3 scale)
		{
			m_scale += scale;
		}
};