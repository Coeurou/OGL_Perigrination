#pragma once

#include "Vertex.hpp"
#include <array>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

static std::array<gs::Vertex, 6> Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
	gs::Vertex va{ a, glm::u8vec2(0,0) };
	gs::Vertex vb{ b, glm::u8vec2(1,0) };
	gs::Vertex vc{ c, glm::u8vec2(1,1) };
	gs::Vertex vd{ d, glm::u8vec2(0,1) };
	std::array<gs::Vertex, 6> res = { va, vb, vc,  va, vc, vd };
	return res;
}

static void RotatePositions(std::vector<gs::Vertex> vertices, float angle, glm::vec3 axis)
{
	glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), angle, axis);
	std::for_each(vertices.begin(), vertices.end(), [&](gs::Vertex& v) { v.position = (rotationMat * glm::vec4(v.position, 1.0f)); });
}