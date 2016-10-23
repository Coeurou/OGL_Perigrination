//
//  OGLDepthPicking.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Stage.hpp"
#include "Camera.hpp"
#include "OGLRenderableCube.hpp"
#include "OGLRenderableQuad.hpp"

class OGLColorPicking : public gs::Stage
{
	bool hasSubscribed = false;
	int selectedBox = -1;
	const size_t NBVERTICESX = 100;
	const size_t NBVERTICESZ = 100;
	const size_t WORLDSIZEX = 1000;
	const size_t WORLDSIZEZ = 1000;

	gs::Camera camera;
    OGLRenderableCube cube;
	std::vector<glm::vec3> boxPositions;
   
	struct LineGround
	{
		gs::VertexArray vao;
		gs::VertexBuffer vbo;
		gs::VertexBuffer ibo;
		
		LineGround() : vbo(GL_ARRAY_BUFFER), ibo(GL_ELEMENT_ARRAY_BUFFER) {}
	} ground;

public:
	OGLColorPicking();
    ~OGLColorPicking();
    
    bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;
    
private:
    bool InitGUI() override;
	void OnEvent(gs::Event e) override;
	void OnMouseButtonDown(gs::Event e);
	void InitGround(size_t xCount, size_t zCount, size_t worldSizeX, size_t worldSizeZ);
};
/* OGLDepthPicking_hpp */
