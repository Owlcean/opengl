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
		//�� ������������
		GLCall(glEnableVertexAttribArray(i));

		//����openlgl��ν�����������,��vbo��vao
		//arg1:����(attrib)������(index),shaderͨ��������֪��ȡ�����������͵����ԣ���0Ϊ�����position;��Χ�� 0 to GL_MAX_VERTEX_ATTRIBS
		//arg2:ÿ�����Զ�Ӧ��ά�ȣ�����position���ԣ��˴�Ϊxyz������3;
		//arg3:����ֵ��Ӧ���������ͣ���GL_FLOAT;
		//arg4:�Ƿ���Ҫ��һ��;
		//arg5:���������ڶ�������bytes
		//arg6:һ�������У���position/texture coordinate/normal�����ԣ���Ҫ��һ����ʼֵ������opengl���ʵ��Ƕ������һ�����ԣ����Ҫ��ȡposition(3D),���Ǵ�0��ʼ���������ȡ�������꣬���Ǵ�3*sizeof(float)����ʼ����attrib byte offset��
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
