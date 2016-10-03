//
//  OGLRenderableQuad.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLRenderableQuad.hpp"

OGLRenderableQuad::OGLRenderableQuad() : vbo(GL_ARRAY_BUFFER)
{}

OGLRenderableQuad::~OGLRenderableQuad()
{}

bool OGLRenderableQuad::Load(const std::string& meshFilename)
{
    bool res = true;
    
    vao.BindVAO();
    res &= quad.InitVertices(glm::vec3(0));
    
    vbo.BindVBO();
    glBufferData(vbo.GetTarget(), sizeof(gs::Vertex) * quad.GetVertices().size(), quad.GetVertices().data(), GL_STATIC_DRAW);
    
    vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
    vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
    vao.AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, normal));
    
    return res;
}

void OGLRenderableQuad::Render()
{
    vao.BindVAO();
    vbo.BindVBO();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void OGLRenderableQuad::Render(int nbInstances)
{
    vao.BindVAO();
    vbo.BindVBO();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nbInstances);
}
