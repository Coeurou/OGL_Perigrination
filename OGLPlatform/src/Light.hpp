#pragma once

#include <glm/glm.hpp>

namespace gs
{
	struct DirectionnalLight
	{
		glm::vec3 direction;
		glm::vec3 color;
		float ambientIntensity;
		float diffuseIntensity;
	};
}