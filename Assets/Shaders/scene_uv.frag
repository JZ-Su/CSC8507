#version 400 core

uniform vec4 		objectColour;
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
	float shadow = 1.0; // New !
	
	if( IN . shadowProj . w > 0.0) { // New !
		shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );
	vec3 viewTangent = normalize(TBN * viewDir);
	
	vec4 albedo = IN.colour;

	vec2 uv = IN.texCoord * 4;
	float height = texture(heightTex, uv).r;
    uv = uv - (0.5 - height) * viewTangent.xy * 0.01f;
	
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);
	//vec3 normal = IN.normal;

	float metal = texture(metalTex, uv).r;
	float roughness = texture(roughTex, uv).r;
	vec4 aoCol = texture(aoTex,uv);
	float smoothness = 1.0 - roughness;
	float shininess = (1.0 * (1.0 - smoothness) + 80 * smoothness) * smoothness;

	float lambert  = max (0.0 , dot ( incident , normal ));// * 0.9; 
	float halfLambert = (lambert + 1.0) * 0.5;
	float rFactor = max (0.0 , dot ( halfDir , normal ));
	float sFactor = pow ( rFactor , shininess);

	if(hasTexture) {
	 albedo *= texture(mainTex, uv);
	}
	vec3 baseCol = albedo.rgb * (1.0 -metal);
	vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal; 
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	//fragColor.rgb = vec3(0.0, 0.0, 0.0);
	fragColor.rgb = albedo.rgb * 0.08f * halfLambert; //ambient
	
	fragColor.rgb += baseCol * lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += specCol * lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb *= aoCol.rgb;

	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.rgb = ACES_Tonemapping(fragColor.rgb);

	fragColor.a = albedo.a;


//	float distance = length(lightPos - IN.worldPos);
//	float attenuation = 1.0f - clamp(distance / lightRadius, 0.0, 1.0);
//	
//	float specFactor = clamp(dot(halfDir, bumpNormal), 0.0, 1.0);
//	specFactor = pow(specFactor, 60.0);
//	vec3 surface = (diffuse.rgb * lightColour.rgb);
//	fragColour.rgb = surface * lambert * attenuation ;
//	fragColour.rgb += (lightColour.rgb * specFactor)* attenuation * 0.33;
//	fragColour.rgb += surface * 0.1f;
//	fragColour.a = diffuse.a;
}