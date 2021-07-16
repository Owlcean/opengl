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

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)//glew必须在opengl上下文创建后调用
	{
		std::cout << "glew init fail" << std::endl;
	}
	{
		//float vertices[] = {
		//	-0.5f,-0.5f,0.0f,
		//	0.5f,-0.5f,0.0f,
		//	0.5f,0.5f,0.0f,
		//	-0.5f,0.5f,0.0f
		//};

		//unsigned int indices[] = {
		//	0,1,2,
		//	2,3,0
		//};

		//VertexArray va;
		//VertexBuffer vb(vertices, 4*3 * sizeof(float));
		//VertexBufferLayout layout;
		//layout.Push<float>(3);//position
		//va.AddBuffer(vb, layout);

		//IndexBuffer ib(indices, sizeof(indices));


		//float r = 0.0f;
		//float increment = 0.04;

		//Shader shader("res/test.shader");
		//shader.Bind();
		//Render render;

		/*
				BMPUtil bmpLoader;
		GLuint texture = bmpLoader.LoadBMP("res/texture/NiuTou.bmp");

		objModel objLoader("res/model/cube1.obj");
		objLoader.setTexture(texture);
		*/

		Model ourModel("res/model/cube1.obj");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			//render.Clear();

			//shader.SetUniform4f("u_Color", r, 0.8f, 1.0f, 1.0f);
			//render.Draw(va, ib, shader);

			//if (r > 1.0f)
			//	increment = -0.05f;
			//else if (r < 0.0f)
			//	increment = 0.05f;

			//r += increment;



			//glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			////glLoadIdentity();
			////gluLookAt(0.0f, 0.0f, 5.0f, 0.0, 0.0, 0.0, 0.0, 1.0f, 0.0f);

			////glPushMatrix();
			////glTranslatef(0.0f, -5.0f, -8.0f);
			////glScalef(0.1f, 0.1f, 0.1f);
			////glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			////glPushMatrix();
			////glColor4ub(255, 255, 255, 255);
			//objLoader.objDraw();
			////glPopMatrix();

					// render the loaded model
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

			ourModel.Draw();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
		glfwTerminate();
	return 0;
}