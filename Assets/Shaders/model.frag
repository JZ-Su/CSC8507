#version 330 core
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;
uniform sampler2D 	aoTex;
uniform sampler2D 	heightTex;
uniform sampler2DShadow shadowTex;
uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

uniform vec3	cameraPos;

in Vertex {
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

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

void main(void) {
 float shadow = 1.0; // New !
	
	if( IN . shadowProj . w > 0.0) { // New !
		shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );
	vec3 viewTangent = normalize(TBN * viewDir);

	vec2 uv = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
	float height = 0.0;
	height = texture(heightTex, uv).r;
    uv = uv - (0.5 - height) * viewTangent.xy * 0.01f;

	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);

	float lambert  = max (0.0 , dot ( incident , normal ));
	float halfLambert = (lambert + 1.0) * 0.5;

	float metal = 0;
	metal = texture(metalTex, uv).r;
	float roughness = 1;
	roughness = texture(roughTex, uv).r;
	vec4 aoCol = texture(aoTex,uv);
	float smoothness = 1.0 - roughness;
	float shininess = (1.0 * (1.0 - smoothness) + 80 * smoothness) * smoothness;

	float rFactor = max (0.0 , dot ( halfDir , normal ));
	float sFactor = pow ( rFactor , shininess);

	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	
	vec4 albedo = texture(mainTex, uv);
	albedo.rgb = pow(albedo.rgb, vec3(2.2));

	vec3 baseCol = albedo.rgb * (1.0 -metal);
	vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal; 

	fragColour.rgb = albedo.rgb * 0.08f * halfLambert; //ambient
	
	fragColour.rgb += baseCol.rgb * lightColour.rgb * lambert * shadow * atten; //diffuse light
	
	fragColour.rgb +=  specCol.rgb * sFactor * shadow * atten; //specular light
	
	fragColour.rgb = pow(fragColour.rgb, vec3(1.0 / 2.2f));
	fragColour.rgb = ACES_Tonemapping(fragColour.rgb);

	fragColour.a = 1.0;
}
