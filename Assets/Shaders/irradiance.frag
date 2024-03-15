#version 330 core

uniform samplerCube cubeTex;

in Vertex {
	vec3 viewDir;
} IN;

out vec4 fragColour;

void main(void)	{
	vec4 samp = texture(cubeTex,normalize(IN.viewDir));
	fragColour = samp;
}