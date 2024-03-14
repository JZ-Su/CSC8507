#version 400 core

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;
uniform sampler2D 	aoTex;
uniform sampler2D 	heightTex;

uniform vec3	shadowPos;
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

void main(void)
{
	vec3 shadcolor = vec3(0.1,0.3,1);

	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 viewTangent = normalize(TBN * viewDir);
	
	vec4 albedo = IN.colour;

	vec2 uv = IN.texCoord * 3;
	float height = 0.0;
	height = texture(heightTex, uv).r;
    uv = uv - (0.5 - height) * viewTangent.xy * 0.01f;
	
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 0.5;
	normal = normalize(TBN * normal);

	float metal = texture(metalTex, uv).r;
	float roughness = texture(roughTex, uv).r;
	vec4 aoCol = texture(aoTex,uv);

	if(hasTexture) {
	 albedo *= texture(mainTex, uv);
	}

	vec3  shadowDir = normalize ( shadowPos - IN.worldPos);
	float l = max (0.01 , dot ( shadowDir , normal ));
	float halL = (l + 1.0) * 0.5;
	halL = clamp(halL, 0.01, 1.0) * aoCol.r;

	fragColor[0] = vec4(albedo.rgb, 1.0);
	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(metal, roughness, 0.0, 1.0);
	fragColor[3] = aoCol;
	fragColor[4] = vec4(0.7, 0.0, halL, 1.0);
}