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
	glfwMakeContextCurrent(window);//opengl渲染上下文

	if (glewInit() != GLEW_OK)//glew必须在opengl上下文创建后调用
	{
		std::cout << "glew init fail" << std::endl;
	}
	{

		Model ourModel("res/model/nanosuit/nanosuit.obj");
		Shader shader("res/shader/model.txt");

		glm::mat4 p = glm::perspective(90.f, 680.0f / 480.0f, 0.1f, 100.f);
		glm::mat4 m = glm::translate(0.0f, -7.0f, -10.0f)*glm::rotate(0.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 mvp = p * m;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			//draw obj directly 
			glClearColor(0.1f, 0.4f, 0.6f, 1.0f); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
			 glEnable(GL_DEPTH_TEST); 
			
			 glEnable(GL_TEXTURE_2D);
			 shader.Bind();
			 glActiveTexture(GL_TEXTURE0);
			 GLint location = glGetUniformLocation(shader.GetID(), "mvp");
			 glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mvp));

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