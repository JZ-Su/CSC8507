#version 400 core

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;
uniform sampler2D 	aoTex;
uniform sampler2D 	heightTex;

//uniform samplerCubeShadow shadowTex;
//uniform samplerCube shadowTex;

//uniform vec3	lightPos;
//uniform float	lightRadius;
//uniform vec4	lightColour;

uniform vec3	cameraPos;

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

//float ShadowCalculation(vec3 wrldPos)
//{
//	float far_plane = 200;
//    vec3 fragToLight = wrldPos - lightPos; 
//	float closestDepth = texture(shadowTex, normalize(fragToLight)).r;
//	closestDepth*=far_plane;
//	float currentDepth=length(fragToLight);
//	float bias=0.1;
//	float shadow = currentDepth-bias > closestDepth? 0.0 : 1.0;
//
//	return shadow;
//}

void main(void)
{
	//float shadow = ShadowCalculation(IN.worldPos);//= texture(shadowTex, IN.shadowProj)*0.5f;

	//vec3  incident = normalize ( lightPos - IN.worldPos );
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	//vec3 halfDir = normalize ( incident + viewDir );
	vec3 viewTangent = normalize(TBN * viewDir);
	
	vec4 albedo = IN.colour;

	vec2 uv = IN.texCoord;
	float height = 0.0;
//	if(hasTexture) {
//		height = texture(heightTex, uv).r;
//		uv = uv - (0.5 - height) * viewTangent.xy * 0.01f;
//	}

	vec3 normal = IN.normal;
//	if(hasTexture) {
//		normal = texture(normalTex, uv).rgb;
//		normal = normalize(normal * 2.0 - 1.0);
//		normal.xy = normal.xy * 1;
//		normal = normalize(TBN * normal);
//	}

	float metal = 0.0;
	//if(hasTexture) {
		//metal = texture(metalTex, uv).r;
	//}
	float roughness = 1.0;
	//if(hasTexture) {
		//roughness = texture(roughTex, uv).r;
	//}
	vec4 aoCol = vec4(1.0, 1.0, 1.0, 1.0);
//	if(hasTexture) {
//		vec4 aoCol = texture(aoTex,uv);
//	}

	if(hasTexture) {
	 albedo *= texture(mainTex, uv);
	}

	fragColor[0] = albedo;
	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(metal, roughness, 0.0, 1.0);
	fragColor[3] = aoCol;
	fragColor[4] = vec4(0.1, 0.0, 0.0, 1.0);
}