#version 400 core

uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

void main(void)
{
	vec3 scale = vec3(lightRadius);
	vec3 worldPos = (position * scale) + lightPos;
	gl_Position = (projMatrix * viewMatrix) * vec4(worldPos, 1.0);
}