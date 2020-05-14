#pragma once
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "Camera.h"

class Maths
{
	public:
		static glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		{
			glm::mat4 translateMatrix = glm::translate(translation);
			glm::mat4 rotationMatrix = glm::rotate(rotation.z, glm::vec3(0, 0, 1)) * glm::rotate(rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(rotation.x, glm::vec3(1, 0, 0));
			glm::mat4 scaleMatrix = glm::scale(scale);

			glm::mat4 transformationMatrix = translateMatrix * rotationMatrix * scaleMatrix;
			return transformationMatrix;
		}
};