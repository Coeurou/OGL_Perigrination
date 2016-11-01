#pragma once
#include <glm/glm.hpp>

namespace gs
{
	// Axis-Aligned Bounding Box
	struct AABB
	{
		glm::vec3 min;
		glm::vec3 max;
	};
}