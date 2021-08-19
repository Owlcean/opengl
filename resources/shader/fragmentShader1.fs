#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 Texcoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    FragColor = mix(texture(tex1,Texcoord),texture(tex2,Texcoord),0.2);
}