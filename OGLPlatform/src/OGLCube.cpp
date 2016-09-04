#include "OGLCube.hpp"
#include "OGLUtility.hpp"

OGLCube::OGLCube()
{}

OGLCube::~OGLCube() 
{}

bool OGLCube::InitVertices(std::array<glm::vec3, 8> data)
{
    auto front = Quad(data[0], data[1], data[2], data[3]);
    auto back = Quad(data[7], data[4], data[5], data[6]);
    auto left = Quad(data[4], data[0], data[3], data[5]);
    auto right = Quad(data[1], data[7], data[6], data[2]);
    auto top = Quad(data[6], data[5], data[3], data[2]);
    auto bottom = Quad(data[1], data[0], data[4], data[7]);
    
    vertices.insert(std::end(vertices), std::begin(front), std::end(front));
    vertices.insert(std::end(vertices), std::begin(back), std::end(back));
    vertices.insert(std::end(vertices), std::begin(left), std::end(left));
    vertices.insert(std::end(vertices), std::begin(right), std::end(right));
    vertices.insert(std::end(vertices), std::begin(top), std::end(top));
    vertices.insert(std::end(vertices), std::begin(bottom), std::end(bottom));
    
    return (vertices.size() == 36);
}

bool OGLCube::InitVertices(glm::vec3 origin)
{
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    float halfD = depth * 0.5f;
    
    std::array<glm::vec3, 8> geometry {
        glm::vec3(origin.x - halfW, origin.y - halfH, origin.z + halfD),
        glm::vec3(origin.x + halfW, origin.y - halfH, origin.z + halfD),
        glm::vec3(origin.x + halfW, origin.y + halfH, origin.z + halfD),
        glm::vec3(origin.x - halfW, origin.y + halfH, origin.z + halfD),
        glm::vec3(origin.x - halfW, origin.y - halfH, origin.z - halfD),
        glm::vec3(origin.x - halfW, origin.y + halfH, origin.z - halfD),
        glm::vec3(origin.x + halfW, origin.y + halfH, origin.z - halfD),
        glm::vec3(origin.x + halfW, origin.y - halfH, origin.z - halfD)
    };
    
    return InitVertices(geometry);
}