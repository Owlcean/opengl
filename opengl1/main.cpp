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
#include "Scripts/Util/SimpleModelLoader.h"
#include "Scripts/Util/NTRLoader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

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

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)//glew������opengl�����Ĵ��������
	{
		std::cout << "glew init fail" << std::endl;
	}
	{
		float vertices[] = {
			-0.5f,-0.5f,0.0f,
			0.5f,-0.5f,0.0f,
			0.5f,0.5f,0.0f,
			-0.5f,0.5f,0.0f
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		VertexArray va;
		VertexBuffer vb(vertices, 4*3 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);//position
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, sizeof(indices));


		float r = 0.0f;
		float increment = 0.04;

		Shader shader("res/test.shader");
		shader.Bind();
		Render render;

		BMPUtil bmpLoader;
		GLuint texture = bmpLoader.LoadBMP("res/texture/NiuTou.bmp");

		//SimpleModelLoader modelLoader;
		//std::vector<glm::vec3>cube_vertices;
		//modelLoader.LoadSphere("res/model/cube.obj", cube_vertices);
		//			modelLoader.Render(cube_vertices);

		NTRLoader ntrLoader;
		ntrLoader.Init("res/model/Sphere.model");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			render.Clear();

			//shader.SetUniform4f("u_Color",r, 0.8f, 1.0f, 1.0f);
			//render.Draw(va,ib,shader);

			shader.Bind();
			va.Bind();
			ib.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLoadIdentity();
			gluLookAt(0.0f, 0.0f, 5.0f, 0.0, 0.0, 0.0, 0.0, 1.0f, 0.0f);

			glPushMatrix();
			glTranslatef(0.0f, -5.0f, -8.0f);
			glScalef(0.1f, 0.1f, 0.1f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			glPushMatrix();
			glColor4ub(255, 255, 255, 255);
			ntrLoader.Render();
			glPopMatrix();

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
		glfwTerminate();
	return 0;
}