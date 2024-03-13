#version 330 core
uniform sampler2D mainTex;
uniform sampler2D normalTex;
uniform sampler2D 	metalTex;

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
	vec2 flippedTexCoord = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	vec3 normal = texture(normalTex, flippedTexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);

	vec3  shadowDir = normalize ( shadowPos - IN.worldPos );
	float l = max (0.01 , dot ( shadowDir , normal ));
	float halL = (l + 1.0) * 0.5;
	halL = clamp(halL, 0.01, 1.0);
	
	vec4 texColor = texture(mainTex, flippedTexCoord);
	float metal = texture(metalTex, flippedTexCoord).r;

	fragColor[0] = vec4(texColor.rgb, 1.0);
	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(metal, 0.0, 0.0, 1.0);
	fragColor[3] = vec4(0.0, 0.0, 0.0, 1.0);
	fragColor[4] = vec4(0.4, 0.0, halL, 1.0);
}
