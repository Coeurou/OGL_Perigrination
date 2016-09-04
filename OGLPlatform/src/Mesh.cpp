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

	void Mesh::PrepareDraw(Program* program)
	{
		vbo.BindVBO();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		ibo.BindVBO();

		if (program->GetUniform("samplerDiffuse1") != -1) {
			glUniform1i(program->GetUniform("samplerDiffuse1"), 0);
		}
		if (program->GetUniform("samplerDiffuse2") != -1) {
			glUniform1i(program->GetUniform("samplerDiffuse2"), 1);
		}
		if (program->GetUniform("samplerDiffuse3") != -1) {
			glUniform1i(program->GetUniform("samplerDiffuse3"), 2);
		}
		if (program->GetUniform("samplerSpecular1") != -1) {
			glUniform1i(program->GetUniform("samplerSpecular1"), 3);
		}
		if (program->GetUniform("samplerSpecular2") != -1) {
			glUniform1i(program->GetUniform("samplerSpecular2"), 4);
		}

		if (material->ambientColor != glm::vec4(0) && program->GetUniform("material.ambientColor") != -1) {
			glUniform4fv(program->GetUniform("material.ambientColor"), 1, glm::value_ptr(material->ambientColor));
		}
		if (material->diffuseColor != glm::vec4(0) && program->GetUniform("material.diffuseColor") != -1) {
			glUniform4fv(program->GetUniform("material.diffuseColor"), 1, glm::value_ptr(material->diffuseColor));
		}
		if (material->specularColor != glm::vec4(0) && program->GetUniform("material.specularColor") != -1) {
			glUniform4fv(program->GetUniform("material.specularColor"), 1, glm::value_ptr(material->specularColor));
		}
		if (material->shininess > std::numeric_limits<float>::epsilon() && program->GetUniform("material.shininess") != -1) {
			glUniform1f(program->GetUniform("material.shininess"), material->shininess);
		}
		int nbDiffuse = 0;
		int nbSpecular = 3;
		for (size_t i = 0; i < textures.size(); i++)
		{
			if (textures[i]->GetContribution() == LIGHT_CONTRIBUTION::DIFFUSE) {
				textures[i]->BindTexture((GLenum)(GL_TEXTURE0 + nbDiffuse));
				nbDiffuse++;
			}
			else if (textures[i]->GetContribution() == LIGHT_CONTRIBUTION::SPECULAR) {
				textures[i]->BindTexture((GLenum)(GL_TEXTURE0 + nbSpecular));
				nbSpecular++;
			}
		}
	}

	void Mesh::Draw(Program* program)
	{
		PrepareDraw(program);
		glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
	}

	void Mesh::Draw(Program * program, int nbInstances)
	{
		PrepareDraw(program);
		glDrawElementsInstanced(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0, nbInstances);
	}
}
