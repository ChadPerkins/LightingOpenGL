#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // Holds all of OpenGL's type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path; // Diffuse or specular texture
};

class Mesh {
public:
	// Mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}
	void Draw(Shader& shader)
	{
		unsigned int diffuseNum = 1;
		unsigned int specularNum = 1;
		for (unsigned int i = 0; 1 < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Activate the proper texture unit before binding
			// Retrieve the texture number 
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNum++);
			else if (name == "texture_specular")
				number = std::to_string(specularNum++);

			shader.setInt(("material" + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		glActiveTexture(GL_TEXTURE0);

		// Draw the mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	// Render data
	unsigned int VAO, VBO, EBO;

	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Vertex postitions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// Vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// Vertex texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};

#endif