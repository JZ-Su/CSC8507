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
	
	vec4 albedo = IN.colour;

	vec2 uv = IN.texCoord * 2;
	
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);
	normal.xy = normal.xy * 1;

	float roughness = texture(roughTex, uv).r;
	float smoothness = 1.0 - roughness;
	float shininess = max(1.0, 80 * smoothness) * smoothness;

	float lambert  = max (0.0 , dot ( incident , normal )) * 0.9; 
	float rFactor = max (0.0 , dot ( halfDir , normal ));
	float sFactor = pow ( rFactor , shininess);

	if(hasTexture) {
	 albedo *= texture(mainTex, uv);
	}
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	fragColor.rgb += albedo.rgb * lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;


//	float lambert = max(dot(incident, bumpNormal), 0.0f);
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