#version 330 core
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;
uniform sampler2D 	aoTex;
uniform sampler2D 	heightTex;

uniform vec3	cameraPos;

in Vertex {
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

	vec2 uv = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
	float height = 0.0;
	height = texture(heightTex, uv).r;
    uv = uv - (0.5 - height) * viewTangent.xy * 0.01f;

	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);

	float metal = 0;
	metal = texture(metalTex, uv).r;
	float roughness = 1;
	roughness = texture(roughTex, uv).r;
	vec4 aoCol = texture(aoTex,uv);

	vec4 albedo = texture(mainTex, uv);

	fragColor[0] = vec4(albedo.rgb, 1.0);
	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(metal, roughness, 0.0, 1.0);
	fragColor[3] = aoCol;
	fragColor[4] = vec4(0.7, 0.0, 0.0, 1.0);
}
