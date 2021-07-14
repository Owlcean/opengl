#include "VertexArray.h"
#include "Render.h"
#include"VertexBufferLayout.h"
VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
	Bind();
	vb.Bind();
	const auto &elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto &element = elements[i];
		//打开 顶点属性数组
		GLCall(glEnableVertexAttribArray(i));

		//告诉openlgl如何解析顶点数据,绑定vbo和vao
		//arg1:属性(attrib)的索引(index),shader通过索引得知读取的是哪种类型的属性，如0为顶点的position;范围是 0 to GL_MAX_VERTEX_ATTRIBS
		//arg2:每个属性对应的维度，比如position属性，此处为xyz，即是3;
		//arg3:属性值对应的数据类型，如GL_FLOAT;
		//arg4:是否需要归一化;
		//arg5:步长，相邻顶点间隔的bytes
		//arg6:一个顶点中，有position/texture coordinate/normal等属性，需要有一个起始值，告诉opengl访问的是顶点的哪一个属性，如果要读取position(3D),就是从0开始读，如果读取纹理坐标，就是从3*sizeof(float)处开始读（attrib byte offset）
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind()const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()const
{
	GLCall(glBindVertexArray(0));
}
