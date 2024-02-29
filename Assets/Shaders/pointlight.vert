#version 400 core

//uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);
//uniform mat4 shadowMatrix 	= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
//layout(location = 3) in vec3 normal;
//layout(location = 4) in vec4 tangent;

uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

//out Vertex
//{
//	//vec4 shadowProj;
//} OUT;
//
void main(void)
{
	vec3 scale = vec3(lightRadius);
	vec3 worldPos = (position * scale) + lightPos;
	//vec3 worldPos = lightPos;
	gl_Position = (projMatrix * viewMatrix) * vec4(worldPos, 1.0);
	//OUT.shadowProj 	=  shadowMatrix * vec4 ( position,1);

//	mat4 mvp 		  = (projMatrix * viewMatrix * modelMatrix);
//	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));
//
//	OUT.shadowProj 	=  shadowMatrix * vec4 ( position,1);
//	OUT.worldPos 	= ( modelMatrix * vec4 ( position ,1)). xyz ;
//	OUT.normal 		= normalize ( normalMatrix * normalize ( normal ));
//	OUT.tangent = normalize(normalMatrix * normalize(tangent.xyz));
//	OUT.binormal = cross(OUT.tangent, OUT.normal) * tangent.w;
//	
//	OUT.texCoord	= texCoord;
//	OUT.colour		= objectColour;
//
//	if(hasVertexColours) {
//		OUT.colour		= objectColour * colour;
//	}
//	gl_Position		= mvp * vec4(position, 1.0);

}