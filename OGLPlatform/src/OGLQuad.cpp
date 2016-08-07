#include "OGLQuad.hpp"


OGLQuad::OGLQuad()
{}

OGLQuad::~OGLQuad() 
{}

bool OGLQuad::InitVertices(glm::vec3 origin)
{
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    
    gs::Vertex a { glm::vec3(origin.x + halfW, origin.y - halfH, 0.5f), glm::vec2(width, 0.0f) };
    gs::Vertex b { glm::vec3(origin.x - halfW, origin.y - halfH, 0.5f), glm::vec2(0.0f, 0.0f) };
    gs::Vertex c { glm::vec3(origin.x + halfW, origin.y + halfH, 0.5f), glm::vec2(width, height) };
    gs::Vertex d { glm::vec3(origin.x - halfW, origin.y + halfH, 0.5f), glm::vec2(0.0f, height) };
    
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    vertices.push_back(d);
    
	return (vertices.size() == 4);
}