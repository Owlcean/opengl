#include "BMPUtil.h"
#include <fstream>
#include "../Render.h"
GLuint  BMPUtil::LoadBMP(const char* imagepath)
{
	FILE * file = fopen(imagepath, "rb");
	ASSERT(fread(header, 1, 54, file) == 54);//54字節文件頭 標示是否為bmp文件、圖像大小、像素位
	ASSERT(header[0] == 'B'&&header[1] == 'M');

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	//buffer
	data = new unsigned char[imageSize];
	//set buffer data
	fread(data, 1, imageSize, file);
	//close file
	fclose(file);

	GLuint textureID;//create texture
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);//bind texture
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);	//actually bmp save BGR,not RGB

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}