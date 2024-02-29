#version 400 core

//uniform vec4 		objectColour;
uniform sampler2D 	colorTex;
uniform sampler2D 	lightDiffTex;
uniform sampler2D 	lightSpecTex;
//uniform sampler2D 	metalTex;
//uniform sampler2D 	roughTex;
//uniform sampler2D 	aoTex;
//uniform sampler2D 	heightTex;
//uniform sampler2DShadow shadowTex;
//
//uniform vec3	lightPos;
//uniform float	lightRadius;
//uniform vec4	lightColour;
//
//uniform vec3	cameraPos;
//
//uniform bool hasTexture;

in Vertex
{
	//vec4 colour;
	vec2 texCoord;
	//vec4 shadowProj;
//	vec3 normal;
//	vec3 tangent;
//	vec3 binormal;
//	vec3 worldPos;
} IN;

out vec4 fragColor;
 vec3 ACES_Tonemapping(vec3 x)
 {
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	vec3 encode_color = (x * (a * x + b)) / (x * (c * x + d) + e);
	encode_color.x = max (0.0, encode_color.x);
	encode_color.x = min (1.0, encode_color.x);
	encode_color.y = max (0.0, encode_color.y);
	encode_color.y = min (1.0, encode_color.y);
	encode_color.z = max (0.0, encode_color.z);
	encode_color.z = min (1.0, encode_color.z);
	return encode_color;
 }

void main(void)
{

	vec4 albedo = texture(colorTex, IN.texCoord);
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	vec4 lightDiff = texture(lightDiffTex, IN.texCoord);
	vec4 lightSpec = texture(lightSpecTex, IN.texCoord);
	lightDiff.rgb = pow(lightDiff.rgb, vec3(2.2));
	lightSpec.rgb = pow(lightSpec.rgb, vec3(2.2));

	fragColor.rgb = albedo.rgb * 0.1f * vec3(0.1,0.3,1.0);
	fragColor.rgb += lightDiff.rgb;
	fragColor.rgb += lightSpec.rgb;
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	fragColor.rgb = ACES_Tonemapping(fragColor.rgb);
	fragColor.a = lightDiff.a;
	if(fragColor.a == 0){
		fragColor.a = 1 - albedo.a;
	}
}