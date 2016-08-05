#pragma once

#include "OGLObjResizable.hpp"
#include <array>

class OGLTriangle : public OGLObjResizable
{
public:
	OGLTriangle();
	~OGLTriangle();

    bool InitVertices(glm::vec3 origin);
    bool InitVertices(std::array<OGLVertex,3> data);
};
