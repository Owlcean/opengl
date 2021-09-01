#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model_nano;
uniform mat4 view_nano;
uniform mat4 projection_nano;
out vec3 Normal;

void main()
{
    TexCoords = aTexCoords;    
	    Normal = mat3(transpose(inverse(model_nano))) * aNormal;  //ĿǰƬ����ɫ����ļ��㶼��������ռ������н��еġ�����Ӧ�ðѷ�����Ҳת��Ϊ����ռ�����
    gl_Position = projection_nano * view_nano * model_nano * vec4(aPos, 1.0);
}