#pragma once
#include <vector>

typedef float Vector3[3];
class NTRLoader {
	struct Vertex {
		float mPosition[4];
		float mTexcoord[4];
		float mNormal[4];
		float mTangent[4];
	};

public:
	void Init(const char * path);
	Vertex* mVertexes;
	int mVertexCount;
	unsigned int* mIndexes;
	int mIndexCount;
	int mFrameRate;
	int mAnimationFrameCount;
	int mBoneCount;

public:
	void Update();
	void Render();
};
