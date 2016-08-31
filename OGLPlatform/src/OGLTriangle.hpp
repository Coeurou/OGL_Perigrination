#pragma once

#include "OGLObjResizable.hpp"
#include <array>

class OGLTriangle : public OGLObjResizable
{
public:
	OGLTriangle();
	~OGLTriangle();

    bool InitVertices(glm::vec3 origin) override;
    bool InitVertices(std::array<gs::Vertex,3> data);
};
