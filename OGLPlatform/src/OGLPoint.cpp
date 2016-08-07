#include "OGLPoint.hpp"
#include <iostream>


OGLPoint::OGLPoint()
{}

OGLPoint::OGLPoint(float size)
{
    width = size;
    glPointSize(width);
}

OGLPoint::~OGLPoint()
{}

bool OGLPoint::InitVertices(glm::vec3 origin)
{
    vertices.push_back(gs::Vertex{ origin, glm::vec2() });
    return (vertices.size() == 1);
}