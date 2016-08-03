#pragma once

#include "OGLVertex.hpp"
#include <array>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

static std::array<OGLVertex, 6> Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
	OGLVertex va{ a, glm::u8vec2(0,0) };
	OGLVertex vb{ b, glm::u8vec2(1,0) };
	OGLVertex vc{ c, glm::u8vec2(1,1) };
	OGLVertex vd{ d, glm::u8vec2(0,1) };
	std::array<OGLVertex, 6> res = { va, vb, vc,  va, vc, vd };
	return res;
}

static void RotatePositions(std::vector<OGLVertex> vertices, float angle, glm::vec3 axis)
{
	glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), angle, axis);
	std::for_each(vertices.begin(), vertices.end(), [&](OGLVertex& v) { v.position = (rotationMat * glm::vec4(v.position, 1.0f)); });
}