#version 330 core
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;

uniform vec3	shadowPos;
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
	
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);
	vec4 albedo = texture(mainTex, uv);
	float metal = 0;
	float roughness = 1;
	vec3  shadowDir = normalize ( shadowPos - IN.worldPos );
	float l = max (0.01 , dot ( shadowDir , normal ));
	float halL = (l + 1.0) * 0.5;
	halL = clamp(halL, 0.01, 1.0);

	if(albedo.a == 1)
	{
	vec4 mixCol = texture(metalTex, uv);
	metal = mixCol.g;
	roughness = mixCol.r;
	float skin = 1.0 - mixCol.b;

	fragColor[2] = vec4(metal, roughness, skin, 1.0);
	fragColor[3] = vec4(0.0, 0.0, 0.0, 0.0);
	fragColor[4] = vec4(0.2, 0.0, halL, 1.0);
	}
	else{
    vec2 noiseUV = uv * (2, 1);
    float noise = texture(metalTex, noiseUV).r - 0.5;

	fragColor[2] = vec4(IN.tangent * 0.5 + 0.5, 1.0);
	fragColor[3] = vec4(IN.binormal * 0.5 + 0.5, 1.0);
	fragColor[4] = vec4(0.3, noise, halL, 1.0);
	}

	fragColor[0] = vec4(albedo.rgb, 1.0);
	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
}