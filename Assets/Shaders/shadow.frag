#version 400 core

uniform sampler2D shadowTex;

in Vertex
{
	vec4 FragPos;
} IN;

out vec4 fragColor;

void main(void)
{

	//fragColor = vec4(1,1,1,1);
	fragColor = texture(shadowTex, IN.FragPos);
}