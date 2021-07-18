#include <GL/glew.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Scripts/Render.h"
#include "Scripts/VertexBuffer.h"
#include "Scripts/IndexBuffer.h"
#include "Scripts/VertexArray.h"
#include "Scripts/Shader.h"
#include "Scripts/VertexBufferLayout.h"
#include "Scripts/Util/BMPUtil.h"
#include "Scripts/Util/objModel.h"
#include "Scripts/Assimp/Model.h"
#include "Scripts/BakeKeyFrame/BakedKeyFrameRenderer.h"

#pragma comment(lib,"glu32.lib")
//YOGO
int main(void)
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);//opengl��Ⱦ������

	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)//glew������opengl�����Ĵ��������
	{
		std::cout << "glew init fail" << std::endl;
	}
	{

		Model ourModel("res/model/nanosuit/nanosuit.obj");
		Shader shader("res/shader/model.txt");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			//draw obj directly 
			glClearColor(0.1f, 0.4f, 0.6f, 1.0f); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
			glLoadIdentity(); glEnable(GL_DEPTH_TEST); 
			glPushMatrix(); 
			glTranslatef(0.0f, -3.0f, -10.0f); 
			glRotatef(0.0f, 0.0f, 1.0f, 0.0f); 
			glScalef(0.5f, 0.5f, 0.5f);
			//shader.Bind();
			ourModel.Draw(shader);

			glPopMatrix();
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
		glfwTerminate();
	return 0;
}