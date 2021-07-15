#pragma once
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <iostream>

struct Float3       //���λ����Ϣ�ͷ�����Ϣ����������
{
	float Data[3];     //x,y,z
};

struct Float2      //�������������������
{
	float Data[2];   //u,v
};

struct Face          //����Ϣ
{
	int vertex[3][3];       //�����㹹��һ����  ÿ����������������Ϣ
};

class objModel
{
public:
	objModel(const char * objFileName);
	void setTexture(GLuint texture);  //��obj�ļ���������
	void objDraw();

private:
	std::vector<Float3> mLocation;   //λ����Ϣ
	std::vector<Float3> mNormal;     //������Ϣ
	std::vector<Float2> mTexcoord;   //����������Ϣ
	std::vector<Face> mFace;         //����Ϣ
	GLuint mTexture;            //ģ������	
};