#version 400 core

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;
uniform sampler2D 	aoTex;
uniform sampler2D 	heightTex;

//uniform samplerCubeShadow shadowTex;
uniform samplerCube shadowTex;

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

out vec4 fragColor ;

float ShadowCalculation(vec3 wrldPos)
{
	float far_plane = 200;
    vec3 fragToLight = wrldPos - lightPos; 
	float closestDepth = texture(shadowTex, normalize(fragToLight)).r;
	closestDepth*=far_plane;
	float currentDepth=length(fragToLight);
	float bias=0.1;
	float shadow = currentDepth-bias > closestDepth? 0.0 : 1.0;

	return shadow;
}

void main(void)
{
	float shadow = ShadowCalculation(IN.worldPos);//= texture(shadowTex, IN.shadowProj)*0.5f;

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float distance = length(lightPos - IN.worldPos);
	float attenuation = 1.0 - clamp(distance / lightRadius, 0.0, 1.0);
	
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	vec3 halfDir = normalize ( incident + viewDir );
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	vec3 viewTangent = normalize(TBN * viewDir);
	
	vec4 albedo = IN.colour;

	vec2 uv = IN.texCoord;
	float height = 0.0;
	height = texture(heightTex, uv).r;
    uv = uv - (0.5 - height) * viewTangent.xy * 0.01f;
	
//	vec3 normal = texture(normalTex, uv).rgb;
//	normal = normalize(normal * 2.0 - 1.0);
//	normal.xy = normal.xy * 1;
//	normal = normalize(TBN * normal);
	vec3 normal = IN.normal;

	float metal = 0;
	metal = texture(metalTex, uv).r;
	float roughness = 1;
	roughness = texture(roughTex, uv).r;
	vec4 aoCol = texture(aoTex,uv);
	float smoothness = 1.0 - roughness;
	float shininess = (1.0 * (1.0 - smoothness) + 80 * smoothness) * smoothness;

	float lambert  = max (0.0 , dot ( incident , normal )) * attenuation;// * 0.9; 
	float halfLambert = (lambert + 1.0) * 0.5;
	float rFactor = max (0.0 , dot ( halfDir , normal ));
	float sFactor = pow ( rFactor , shininess);

	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	if(hasTexture) {
	 albedo *= texture(mainTex, uv);
	}
	albedo.rgb = pow(albedo.rgb, vec3(2.2));

	vec3 shadcolor = vec3(0,0.3,1);
	
	fragColor.rgb = albedo.rgb * shadcolor* 0.05f; //ambient
	vec3 baseCol = albedo.rgb * (1.0 -metal);
	
	fragColor.rgb += baseCol * lightColour.rgb * lambert * shadow * atten; //diffuse light
	
	fragColor.rgb += lightColour.rgb * sFactor * shadow * atten; //specular light
	
	//fragColor.rgb *= aoCol.rgb;

	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));

	fragColor.a = albedo.a;

//fragColor.rgb = IN.normal;

	//fragColor = IN.colour;
	
	//fragColor.xy = IN.texCoord.xy;
	
	//fragColor = IN.colour;
}