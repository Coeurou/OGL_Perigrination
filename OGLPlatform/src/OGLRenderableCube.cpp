//
//  OGLRenderableCube.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLRenderableCube.hpp"
#include "OGLUtility.hpp"

OGLRenderableCube::OGLRenderableCube() : vbo(GL_ARRAY_BUFFER)
{}

OGLRenderableCube::~OGLRenderableCube()
{}

bool OGLRenderableCube::Load(const std::string& meshFilename)
{
    bool res = true;
    
    vao.BindVAO();
    res &= cube.InitVertices(glm::vec3(0));
    
    vbo.BindVBO();
    glBufferData(vbo.GetTarget(), sizeof(gs::Vertex) * cube.GetVertices().size(), cube.GetVertices().data(), GL_STATIC_DRAW);
    
    vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
    vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
    vao.AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, normal));
    
    return res;
}

void OGLRenderableCube::Render()
{
    vao.BindVAO();
    vbo.BindVBO();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void OGLRenderableCube::Render(int nbInstances)
{
    vao.BindVAO();
    vbo.BindVBO();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, nbInstances);
}
