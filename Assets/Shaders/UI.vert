#version 400 core

layout(location = 0) in vec3 position1;
layout(location = 1) in vec3 position2;
layout(location = 2) in vec3 position3;
layout(location = 3) in vec3 position4;
layout(location = 4) in vec4 colour;
layout(location = 5) in vec2 texCoord;

uniform mat4 viewProjMatrix = mat4(1);

out Vertex
{
	vec4 colour;
	vec2 texCoord;
} OUT;

void main(void)
{
	gl_Position		= viewProjMatrix * vec4(position1, 1.0);
	OUT.texCoord	= texCoord;
	OUT.colour		= colour;
}