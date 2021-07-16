#pragma once
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <iostream>

struct Float3       //点的位置信息和法线信息的数据类型
{
	float Data[3];     //x,y,z
};

struct Float2      //点的纹理坐标数据类型
{
	float Data[2];   //u,v
};

struct Face          //面信息
{
	int vertex[3][3];       //三个点构成一个面  每个点有三个索引信息
};

class objModel
{
public:
	objModel(const char * objFileName);
	void setTexture(GLuint texture);  //从obj文件创建纹理
	void objDraw();

private:
	std::vector<Float3> mLocation;   //位置信息
	std::vector<Float3> mNormal;     //法线信息
	std::vector<Float2> mTexcoord;   //纹理坐标信息
	std::vector<Face> mFace;         //面信息
	GLuint mTexture;            //模型纹理	
};