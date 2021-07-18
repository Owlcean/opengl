#pragma once
#include <unordered_map>
#include <set>
#include <GL/glew.h>
#include <vector>
#pragma comment(lib, "winmm.lib ")
struct Vertex1 {
	float mPosition[4];
	float mTexcoord[4];
	float mNormal[4];
	float mTangent[4];
};
struct KeyFrame {
	Vertex1* mVertexes;
	int mVertexCount;
	unsigned int* mIndexes;
	int mIndexCount;
};
class BakedKeyFrameRenderer {
public:
	void Init(const char * path);
	KeyFrame* mKeyFrames;
	int mFrameRate;
	int mAnimationFrameCount;
	int mCurrentKeyFrame;
	GLuint mTexture;            //Ä£ÐÍÎÆÀí	
public:
	void Update();
	void Render();
	void SetTexture(GLuint texture);
    std::vector<GLfloat> GetVerticesSet();
};