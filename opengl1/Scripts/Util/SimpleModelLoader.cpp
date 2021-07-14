#include "SimpleModelLoader.h"
#include <fstream>
#include <GL/glew.h>

/*
Sphere.model 的数据存储格式如下： 
-顶点数量(int) 
-顶点数据块(见教程代码里的 Vertex 定义) 
-索引数量(int) 
-索引数据块(unsigned int)
*/
void SimpleModelLoader::LoadSphere(const char* path) 
{
	FILE * pFile = fopen(path, "rb");
	fread(&mVertexCount, 1, sizeof(int), pFile);
	mVertexes = new Vertex[mVertexCount];
	fread(mVertexes, 1, sizeof(Vertex) * mVertexCount, pFile);
	fread(&mIndexCount, 1, sizeof(int), pFile);
	mIndexes = new unsigned int[mIndexCount];
	fread(mIndexes, 1, sizeof(unsigned int) * mIndexCount, pFile);
	fclose(pFile);
}

void SimpleModelLoader::Render()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	Vector3* vertexes = new Vector3[mVertexCount];
	for (int index = 0; index < mIndexCount; ++index) {
		glVertex3fv(vertexes[mIndexes[index]]);
	}
	delete[]vertexes;
	glEnd();
}