#version 400 core

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;
uniform sampler2D 	aoTex;
uniform sampler2D 	heightTex;

uniform vec3	cameraPos;
uniform vec3	shadowPos;
uniform bool hasTexture;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 fragColor[5];

void main(void)
{
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 viewTangent = normalize(TBN * viewDir);
	
	vec4 albedo = IN.colour;

	vec2 uv = IN.texCoord;

	vec3 normal = IN.normal;

	if(hasTexture) {
	 albedo *= texture(mainTex, uv);
	}

	float fresnel = max(0.0, dot(normal, viewDir));
	fresnel = 1.0 - fresnel;
	fresnel = pow(fresnel, 2) * 3;

	fragColor[0] = vec4(albedo.rgb, 1.0);
	fragColor[0].rgb += vec3(0.5, 0.5, 0.5) * fresnel;

	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(0.0, 1.0, 0.0, 1.0);
	fragColor[3] = vec4(0.0, 0.0, 0.0, 1.0);
	fragColor[4] = vec4(0.5, 0.0, 2.0, 1.0);
}