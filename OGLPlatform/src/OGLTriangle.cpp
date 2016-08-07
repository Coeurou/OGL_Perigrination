#include "OGLTriangle.hpp"

OGLTriangle::OGLTriangle()
{}


OGLTriangle::~OGLTriangle()
{}

bool OGLTriangle::InitVertices(glm::vec3 origin)
{
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    gs::Vertex a { glm::vec3(origin.x - halfW, origin.y - halfH, 0.5f), glm::vec2(0.0f, 0.0f) };
    gs::Vertex b { glm::vec3(origin.x + halfW, origin.y - halfH, 0.5f), glm::vec2(1.0f, 0.0f) };
    gs::Vertex c { glm::vec3(origin.x + halfW, origin.y + halfH, 0.5f), glm::vec2(1.0f, 1.0f) };
    
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    return vertices.size() == 3;
}

bool OGLTriangle::InitVertices(std::array<gs::Vertex,3> data)
{
    for (auto vertex : data) {
        vertices.push_back(vertex);
    }

    return vertices.size() == 3;
}