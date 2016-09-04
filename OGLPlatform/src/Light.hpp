#pragma once

#include <glm/glm.hpp>

namespace gs
{
	struct DirectionnalLight
	{
		glm::vec3 direction;
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
	};

	struct PointLight
	{
		glm::vec3 position;
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;

		float constantAttenuation;
		float linearAttenuation;
		float quadraticAttenuation;
	};

	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;

		float innerCutOff;
		float outerCutOff;
	};
}