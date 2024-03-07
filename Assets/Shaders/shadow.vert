#version 400 core

uniform mat4 mvpMatrix = mat4(1.0f);
uniform bool isAnimation = false;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

layout(location = 5) in vec4 jointWeights;
layout(location = 6) in ivec4 jointIndices;

uniform mat4 joints [128];

void main(void)
{
	if(!isAnimation){
		gl_Position	= mvpMatrix * vec4(position, 1.0);
	}
	else{
		vec4 localPos = vec4 (position,1.0f);
		vec4 skelPos = vec4 (0,0,0,0);
		for (int i = 0; i < 4; ++i ) {
			int jointIndex = jointIndices[i];
			float jointWeight = jointWeights[i];
			skelPos += joints[jointIndex] * localPos * jointWeight ;
		}
		gl_Position = mvpMatrix * vec4 (skelPos.xyz,1.0f);
	}
}