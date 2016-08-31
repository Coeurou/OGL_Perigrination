#include "Mesh.hpp"
#include "Program.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace gs
{
	Mesh::Mesh() : vbo(GL_ARRAY_BUFFER), ibo(GL_ELEMENT_ARRAY_BUFFER)
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::InitGL(std::vector<Vertex> vertices, std::vector<GLuint> indices)
	{
		vbo.BindVBO();
		glBufferData(vbo.GetTarget(), sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		nbIndices = (int)indices.size();
		ibo.BindVBO();
		glBufferData(ibo.GetTarget(), sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}	

	void Mesh::Draw(Program* program)
	{
		vbo.BindVBO();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		ibo.BindVBO();			

		glUniform4fv(program->GetUniform("material.ambientColor"), 1, glm::value_ptr(material->GetAmbientColor()));
		glUniform4fv(program->GetUniform("material.diffuseColor"), 1, glm::value_ptr(material->GetDiffuseColor()));

		auto& textures = material->GetTextures();
		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i]->BindTexture((GLenum)(GL_TEXTURE0+i));
		}		

		glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
	}
}
