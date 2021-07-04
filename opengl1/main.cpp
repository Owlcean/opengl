#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	std::cout << stream._Stdstr << std::endl;
	while (getline(stream, line))
	{
		std::cout << line << std::endl;
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if(line.find("fragment") != std::string::npos){
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type]<< line << '\n';
		}
	}
	return {  ss[0].str(),ss[1].str() };

}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "fail to compile "<<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
	}
	return id;
}

static unsigned int CreateShader(const std::string&vertexShader,const std::string &fragmentShader){
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}


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
	glfwMakeContextCurrent(window);//opengl渲染上下文

	if (glewInit() != GLEW_OK)//glew必须在opengl上下文创建后调用
	{
		std::cout << "glew init fail" << std::endl;
	}

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




	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//将生成的buffer绑定到opengl状态机的某一个buffer上
	
	//给glBufferData指定一个buffer，
	//arg1:Buffer Binding Target;
	//arg2:size of this buffer;
	//arg3:pointer to be copied into the initial data;
	//arg4:pattern of the data store;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//打开 顶点属性数组
	glEnableVertexAttribArray(0);

	//告诉openlgl如何解析顶点数据
	//arg1:属性(attrib)的索引(index),shader通过索引得知读取的是哪种类型的属性，如0为顶点的position;
	//arg2:每个属性对应的维度，比如position属性，此处为xyz，即是3;
	//arg3:属性值对应的数据类型，如GL_FLOAT;
	//arg4:是否需要归一化;
	//arg5:步长，相邻顶点间隔的bytes
	//arg6:一个顶点中，有position/texture coordinate/normal等属性，需要有一个起始值，告诉opengl访问的是顶点的哪一个属性，如果要读取position(3D),就是从0开始读，如果读取纹理坐标，就是从3*sizeof(float)处开始读（attrib byte offset）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	ShaderProgramSource source = ParseShader("res/test.shader");

	std::cout << "vertex" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "fragment" << std::endl;
	std::cout << source.FragmentSource << std::endl;

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//render primitives from array data
		//arg1:mode
		//arg2:number of elements to be rendererd
		//arg3:the type of the values in indices
		//arg4:an offset of the first index in the array in the data store of the buffer currently bound to the GL_ELEMENT_ARRAY_BUFFER target
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}