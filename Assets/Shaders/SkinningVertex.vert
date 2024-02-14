#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec4 tangent;
layout(location = 5) in vec4 jointWeights;
layout(location = 6) in ivec4 jointIndices;

uniform mat4 joints [128];

out Vertex {
	vec2 texCoord;
} OUT;

void main (void) {
	vec4 localPos = vec4 (position,1.0f);
	vec4 skelPos = vec4 (0,0,0,0);
	for (int i = 0; i < 4; ++i ) {
	int jointIndex = jointIndices[i];
	float jointWeight = jointWeights[i];
		skelPos += joints[jointIndex] * localPos * jointWeight ;
	}
	mat4 mvp = projMatrix * viewMatrix * modelMatrix ;
	gl_Position = mvp * vec4 (skelPos.xyz,1.0f);
	//gl_Position = mvp * vec4 (position,1.0f);

	OUT.texCoord = texCoord;
}