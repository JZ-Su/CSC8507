#version 400 core

uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);
uniform mat4 shadowMatrix 	= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec4 tangent;

uniform vec4 		objectColour = vec4(1,1,1,1);

uniform bool hasVertexColours = false;

out Vertex
{
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} OUT;

void main(void)
{
	vec4 localPos = vec4 (position,1.0f);
	
	mat4 mvp = projMatrix * viewMatrix * modelMatrix ;
	gl_Position = mvp * localPos;
	//gl_Position = mvp * vec4 (position,1.0f);

	OUT.texCoord = texCoord;
	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));
	OUT.normal 		= normalize ( normalMatrix * normalize ( normal ));
	OUT.tangent = normalize(normalMatrix * normalize(tangent.xyz));
	OUT.binormal = cross(OUT.tangent, OUT.normal) * tangent.w;
	OUT.worldPos 	= ( modelMatrix * vec4 ( position ,1)). xyz ;
}