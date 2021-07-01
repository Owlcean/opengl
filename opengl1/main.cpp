#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//YOGO
int main(void)
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);//opengl��Ⱦ������

	if (glewInit() != GLEW_OK)//glew������opengl�����Ĵ��������
	{
		std::cout << "glew init fail" << std::endl;
	}

	float vertices[9] = {
		-0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f,
		0.5f,-0.5f,0.0f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//�����ɵ�buffer�󶨵�opengl״̬����ĳһ��buffer��
	
	//��glBufferDataָ��һ��buffer��
	//arg1:Buffer Binding Target;
	//arg2:size of this buffer;
	//arg3:pointer to be copied into the initial data;
	//arg4:pattern of the data store;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//�� ������������
	glEnableVertexAttribArray(0);

	//����openlgl��ν�����������
	//arg1:����(attrib)������(index),shaderͨ��������֪��ȡ�����������͵����ԣ���0Ϊ�����position;
	//arg2:ÿ�����Զ�Ӧ��ά�ȣ�����position���ԣ��˴�Ϊxyz������3;
	//arg3:����ֵ��Ӧ���������ͣ���GL_FLOAT;
	//arg4:�Ƿ���Ҫ��һ��;
	//arg5:���������ڶ�������bytes
	//arg6:һ�������У���position/texture coordinate/normal�����ԣ���Ҫ��һ����ʼֵ������opengl���ʵ��Ƕ������һ�����ԣ����Ҫ��ȡposition(3D),���Ǵ�0��ʼ���������ȡ�������꣬���Ǵ�3*sizeof(float)����ʼ����attrib byte offset��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}