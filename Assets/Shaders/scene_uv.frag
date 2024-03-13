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

//uniform vec3	lightpos;
//uniform float	lightradius;
//uniform vec4	lightcolour;

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
//    //float closestDepth = texture(shadowTex, vec4(fragToLight,1));
//	float closestDepth = texture(shadowTex, normalize(fragToLight)).r;
//	closestDepth*=far_plane;
//	float currentDepth=length(fragToLight);
//	float bias=0.1;//5;
//	//float bias=0.0;
//	float shadow = currentDepth-bias > closestDepth? 0.0 : 1.0;
//
//	//float test = length(closestDepth - currentDepth) / 100.0f;
//
//	return shadow;
//}



void main(void)
{
	//float shadow = 1.0; 
	//shadow = ShadowCalculation(IN.worldPos);
	//if( IN.shadowProj.w > 0.0) { 
		//= texture(shadowTex, IN.shadowProj)*0.5f;
	//}
		//fragColor = vec4(shadow,shadow,shadow,1);
	//return;

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
	//albedo.rgb = pow(albedo.rgb, vec3(2.2));

	//vec3 baseCol = albedo.rgb * (1.0 -metal);
	//vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal; 

	fragColor[0] = vec4(albedo.rgb, 1.0);
	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(metal, roughness, 0.0, 1.0);
	fragColor[3] = aoCol;
	fragColor[4] = vec4(0.1, 0.0, 0.0, 1.0);


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