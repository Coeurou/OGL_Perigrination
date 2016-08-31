//
//  Mesh.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <memory>
#include "OGLRenderableObject.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace gs
{
	class Mesh;
	class Material;

	class Model : public OGLRenderableObject
	{
	public:
		Model();
		~Model();

		GLuint GetVAO() const { return vao.get(); }

		bool Load(const std::string& meshFilename) override;
		void Render(Program* program) override;

	private:	
		bool InitFromScene(const aiScene* scene);
		bool InitMesh(unsigned int index, const aiMesh* mesh, Material* material);
		bool InitMaterials(const aiScene* scene);
		bool InitMaterialByType(const aiMaterial* material, const aiTextureType texType, unsigned int startIndex);

		std::vector<Mesh> subMeshes;
		std::vector<std::shared_ptr<Material>> materials;
		VertexArray vao;
	};
}
/* OGLTexture_hpp */
