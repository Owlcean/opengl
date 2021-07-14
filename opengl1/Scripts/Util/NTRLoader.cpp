#include "NTRLoader.h"
#include <fstream>
#include <GL/glew.h>

void NTRLoader::Init(const char* path) {
	FILE* pFile = fopen(path, "rb");
	fread(&mFrameRate, 1, sizeof(int), pFile);
	fread(&mAnimationFrameCount, 1, sizeof(int), pFile);
	fread(&mBoneCount, 1, sizeof(int), pFile);
	fread(&mVertexCount, 1, sizeof(int), pFile);
	mVertexes = new Vertex[mVertexCount];
	fread(mVertexes, 1, sizeof(Vertex) * mVertexCount, pFile);
	fread(&mIndexCount, 1, sizeof(int), pFile);
	mIndexes = new unsigned int[mIndexCount];
	fread(mIndexes, 1, sizeof(unsigned int) * mIndexCount, pFile);
	fclose(pFile);
}

void NTRLoader::Render()
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