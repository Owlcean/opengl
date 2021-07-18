#include "BakedKeyFrameRenderer.h"
#include <windows.h>
#include <gl/GL.h>
#pragma warning(disable:4996)
void BakedKeyFrameRenderer::Init(const char* path) {
	FILE* pFile = fopen(path, "rb");
	fread(&mFrameRate, 1, sizeof(int), pFile);
	fread(&mAnimationFrameCount, 1, sizeof(int), pFile);
	mKeyFrames = new KeyFrame[mAnimationFrameCount];
	for (int i = 0; i < mAnimationFrameCount; ++i) {
		fread(&mKeyFrames[i].mVertexCount, 1, sizeof(int), pFile);
		mKeyFrames[i].mVertexes = new Vertex1[mKeyFrames[i].mVertexCount];
		fread(mKeyFrames[i].mVertexes, 1, sizeof(Vertex1) * mKeyFrames[i].mVertexCount, pFile);
		fread(&mKeyFrames[i].mIndexCount, 1, sizeof(int), pFile);
		mKeyFrames[i].mIndexes = new unsigned int[mKeyFrames[i].mIndexCount];
		fread(mKeyFrames[i].mIndexes, 1, sizeof(unsigned int) * mKeyFrames[i].mIndexCount, pFile);
	}
	fclose(pFile);
}
static float GetFrameTime() {
	static unsigned long lastTime = 0, timeSinceComputerStart = 0;
	timeSinceComputerStart = timeGetTime();
	unsigned long frameTime = lastTime == 0 ? 0 : timeSinceComputerStart - lastTime;
	lastTime = timeSinceComputerStart;
	return float(frameTime) / 1000.0f;
}
void BakedKeyFrameRenderer::Update() {
	static int frame_index = 0;
	static float frameTime = 1.0f / 24.0f;
	static float animationTime = 0.0f;
	float deltaTime = GetFrameTime();
	animationTime += deltaTime;
	while (animationTime > frameTime) {
		animationTime -= frameTime;
		frame_index++;
	}
	if (frame_index >= mAnimationFrameCount) {
		frame_index = 0;
	}
	mCurrentKeyFrame = frame_index;
}

void BakedKeyFrameRenderer::SetTexture(GLuint texture)
{
	mTexture = texture;
}

void BakedKeyFrameRenderer::Render() {
	glColor3f(1.0f, 1.0f, 1.0f);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	glBegin(GL_TRIANGLES);
	for (int index = 0; index < mKeyFrames[mCurrentKeyFrame].mIndexCount; ++index) {
		glVertex3fv(mKeyFrames[mCurrentKeyFrame].mVertexes[mKeyFrames[mCurrentKeyFrame].mIndexes[index]].mPosition);
		//glTexCoord2fv(mKeyFrames[mCurrentKeyFrame].mVertexes[mKeyFrames[mCurrentKeyFrame].mIndexes[index]].mTexcoord);
	}
	glEnd();
}

