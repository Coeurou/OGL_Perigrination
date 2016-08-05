#include "OGLTriangle.hpp"

OGLTriangle::OGLTriangle()
{}


OGLTriangle::~OGLTriangle()
{}

bool OGLTriangle::InitVertices(glm::vec3 origin)
{
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    OGLVertex a { glm::vec3(origin.x - halfW, origin.y - halfH, 0.5f), glm::vec2(0.0f, 0.0f) };
    OGLVertex b { glm::vec3(origin.x + halfW, origin.y - halfH, 0.5f), glm::vec2(1.0f, 0.0f) };
    OGLVertex c { glm::vec3(origin.x + halfW, origin.y + halfH, 0.5f), glm::vec2(1.0f, 1.0f) };
    
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    return vertices.size() == 3;
}

bool OGLTriangle::InitVertices(std::array<OGLVertex,3> data)
{
    for (auto vertex : data) {
        vertices.push_back(vertex);
    }

    return vertices.size() == 3;
}