#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

//uniform mat4 viewProjMatrix = mat4(1);

out Vertex
{
	vec4 colour;
	vec2 texCoord;
} OUT;

//out vec2 TexCoord;

void main(void)
{
	//gl_Position		= viewProjMatrix * vec4(position, 1.0);
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	OUT.texCoord		= texCoord;
	OUT.colour		= colour;
	//TexCoord= texCoord;

}