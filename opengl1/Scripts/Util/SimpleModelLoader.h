#pragma once
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

typedef float Vector3[3];

struct Vertex {
	float mPosition[4];
	float mTexcoord[4];
	float mNormal[4];
	float mTangent[4];
};

class SimpleModelLoader
{
private:
public:


	int mVertexCount;
	Vertex* mVertexes;
	int mIndexCount;
	unsigned int* mIndexes;
	void LoadSphere(const char* path, std::vector<glm::vec3>out_vertices);
	void Render(std::vector<glm::vec3>out_vertices);
};