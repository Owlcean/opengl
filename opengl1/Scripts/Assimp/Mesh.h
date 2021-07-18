#ifndef MESH_H
#define MESH_H
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <string>
#include <vector>
#include <GL/glew.h>
#include "../Shader.h"
struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	unsigned int VAO;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader &shader);
private:
	// render data 
	unsigned int VBO, EBO;
	void setupMesh();
};
#endif
