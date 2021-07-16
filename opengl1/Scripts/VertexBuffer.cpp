#include "VertexBuffer.h"
#include "Render.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);//�����ɵ�buffer�󶨵�opengl״̬����ĳһ��buffer��

	//��glBufferDataָ��һ��buffer��
	//arg1:Buffer Binding Target;
	//arg2:size of this buffer;
	//arg3:pointer to be copied into the initial data;
	//arg4:pattern of the data store;
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}