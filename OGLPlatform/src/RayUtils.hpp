#pragma once
#include <glm/glm.hpp>
#include <algorithm>
#include "Ray.hpp"
#include "AABB.hpp"

static glm::vec2 IntersectBox(gs::Ray ray, gs::AABB box)
{
	glm::vec3 invDir = 1.0f / ray.direction;
	auto tMin = (box.min - ray.origin) * invDir;
	auto tMax = (box.max - ray.origin) * invDir;

	auto t1 = glm::min(tMin, tMax);
	auto t2 = glm::max(tMin, tMax);

	auto tNear = std::max(std::max(t1.x, t1.y), t1.z);
	auto tFar = std::min(std::min(t2.x, t2.y), t2.z);
	return glm::vec2(tNear, tFar);
}