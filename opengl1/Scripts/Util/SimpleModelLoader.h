#pragma once
#include <vector>

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
	void LoadSphere(const char* path);
	void Render();
};