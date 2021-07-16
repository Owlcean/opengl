#include "objModel.h"

unsigned char*LoadFileContent(const char*path, int &filesize)   //�ú����������ȹȽ̳�
//��ȡ�ļ��������ļ����ݣ����ļ���С��ֵ��filesize       
{
	unsigned char*fileContent = nullptr;
	filesize = 0;
	FILE*pFile = fopen(path, "rb");   //�����Ʒ�ʽ��ȡ
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);      //���ļ�ָ���ƶ����ļ�ĩβ
		int nLen = ftell(pFile);        //�����ļ�ͷ���ľ���   //����ָ�ļ���С
		if (nLen > 0)
		{
			rewind(pFile);          //�ƶ����ļ�ͷ��
			fileContent = new unsigned char[nLen + 1];
			//Ϊ�ļ�ָ�뿪�ٿռ�
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			//��pFile�����ݶ���fileContent
			fileContent[nLen] = '\0';         //�ļ�ĩβ����\0
			filesize = nLen;                  //Ϊ�ļ���С��ֵ
		}
		fclose(pFile);
	}
	return fileContent;
};

objModel::objModel(const char * objFileName)
{
	int nFileSize = 0;
	unsigned char*fileContent = LoadFileContent(objFileName, nFileSize);    //��ȡ�ļ�����
	if (fileContent == nullptr)     //�ļ�Ϊ��
	{
		return;
	}
	std::stringstream ssFileContent((char*)fileContent);   //����ȡ�ļ�����
	std::string temp;       //�����޹���Ϣ
	char szoneLine[256];        //��ȡһ�е�����
	while (!ssFileContent.eof())
	{
		memset(szoneLine, 0, 256);        //  ÿ��ѭ����ʼ������szoneLine
		ssFileContent.getline(szoneLine, 256);      //����ȡһ��
		if (strlen(szoneLine) > 0)       //���в�Ϊ��
		{
			if (szoneLine[0] == 'v')     //v��ͷ������
			{
				std::stringstream ssOneLine(szoneLine);        //���ݴ洢������ ���㸳ֵ
				if (szoneLine[1] == 't')       //������Ϣ
				{
					ssOneLine >> temp;     //���ܱ�ʶ�� vt
					Float2 tempTexcoord;
					ssOneLine >> tempTexcoord.Data[0] >> tempTexcoord.Data[1];   //���ݴ�����ʱ������
					mTexcoord.push_back(tempTexcoord);         //��������
					 
				}
				else if (szoneLine[1] == 'n')            //������Ϣ
				{
					ssOneLine >> temp;      //���ձ�ʶ��vn
					Float3 tempNormal;
					ssOneLine >> tempNormal.Data[0] >> tempNormal.Data[1] >> tempNormal.Data[2];
					mNormal.push_back(tempNormal);
				}
				else                          //���λ����Ϣ
				{
					ssOneLine >> temp;
					Float3 tempLocation;
					ssOneLine >> tempLocation.Data[0] >> tempLocation.Data[1] >> tempLocation.Data[2];
					mLocation.push_back(tempLocation);
				}
			}
			else if (szoneLine[0] == 'f')          //����Ϣ
			{
				std::stringstream ssOneLine(szoneLine);     //����ȡһ������
				ssOneLine >> temp; //���ձ�ʶ��f
				//    f��Ϣ    exp�� f 1/1/1 2/2/2 3/3/3      λ������/��������/��������   ������Ƭ �����㹹��һ����
				std::string vertexStr;   //������������
				Face tempFace;
				for (int i = 0; i < 3; ++i)         //ÿ����������
				{
					ssOneLine >> vertexStr;           //�����ж�ȡ���������Ϣ
					size_t pos = vertexStr.find_first_of('/');       //�ҵ���һ��/��λ��      //���ҵ����λ����Ϣ
					std::string locIndexStr = vertexStr.substr(0, pos);       //��ֵ���λ����Ϣ
					size_t pos2 = vertexStr.find_first_of('/', pos + 1);   //�ҵ��ڶ���/   ���ҵ��������������Ϣ
					std::string texIndexSrt = vertexStr.substr(pos + 1, pos2 - 1 - pos);       //��ֵ�������������Ϣ
					std::string norIndexSrt = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);   //��ֵ��ķ�����Ϣ
					tempFace.vertex[i][0] = atoi(locIndexStr.c_str());        //��������Ϣ�� srtingת��Ϊ int     //λ��������Ϣ��ֵ
					tempFace.vertex[i][1] = atoi(texIndexSrt.c_str());         //��������������Ϣ��ֵ
					tempFace.vertex[i][2] = atoi(norIndexSrt.c_str());         //������Ϣ��ֵ
				}
				mFace.push_back(tempFace);
			}
		}   //end ��0��
	}  //end while
	delete fileContent;
}

void objModel::setTexture(GLuint texture)
{
	mTexture = texture;
}

void objModel::objDraw()
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glBegin(GL_TRIANGLES);
	for (auto faceIndex = mFace.begin(); faceIndex != mFace.end(); ++faceIndex)         //ѭ������face��Ϣ
	{
		//��һ����ķ��ߣ�����λ����Ϣ
		glNormal3fv(mNormal[faceIndex->vertex[0][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[0][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[0][0] - 1].Data);
		//�ڶ�����ķ��ߣ�����λ����Ϣ
		glNormal3fv(mNormal[faceIndex->vertex[1][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[1][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[1][0] - 1].Data);
		//��������ķ��ߣ�����λ����Ϣ
		glNormal3fv(mNormal[faceIndex->vertex[2][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[2][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[2][0] - 1].Data);
	}
	glEnd();
}