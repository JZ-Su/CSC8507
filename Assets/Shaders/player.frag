#version 330 core
uniform sampler2D 	mainTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	metalTex;
uniform sampler2D 	roughTex;
//uniform sampler2DShadow shadowTex;
uniform samplerCube shadowTex;
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
	float shadow = ShadowCalculation(IN.worldPos);

	vec3  incident = normalize ( lightPos - IN.worldPos );
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	vec2 uv = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);

	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);

	float lambert  = max (0.0 , dot ( incident , normal ));
	float halfLambert = (lambert + 1.0) * 0.5;

	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	vec4 albedo = texture(mainTex, uv);
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	fragColour.rgb = albedo.rgb * 0.08f * halfLambert; //ambient
	
	if(albedo.a == 1){
	vec4 mixCol = texture(metalTex, uv);
	float metal = mixCol.g;
	float roughness = mixCol.r;
	float skin = 1.0 - mixCol.b;
	float smoothness = 1.0 - roughness;
	float shininess = (1.0 * (1.0 - smoothness) + 80 * smoothness) * smoothness;

	float rFactor = max (0.0 , dot ( halfDir , normal ));
	float sFactor = pow ( rFactor , shininess);

	vec3 baseCol = albedo.rgb * (1.0 -metal);
	vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal; 
	vec3 diffuseCol = baseCol.rgb * lightColour.rgb * lambert * shadow * atten;

	float skinX = clamp(lambert + 0.5, 0.0, 1.0);
	vec2 skinUV = vec2(skinX, 1);
	vec3 skinCol = texture(roughTex, skinUV).rgb;
    skinCol = pow(skinCol, vec3(2.2));
    vec3 skinDiffCol = skinCol * lightColour.rgb * baseCol;

    fragColour.rgb += diffuseCol * (1.0 - skin) + skinDiffCol * skin;
	fragColour.rgb +=  specCol.rgb * sFactor * shadow * atten; //specular light
	}
	else{
	float roughness = 0.5;
    float smoothness = 1.0 - roughness;
    
    fragColour.rgb += lightColour.rgb * albedo.rgb * halfLambert;

    vec2 noiseUV = uv * (2, 1);
    float noise = texture(metalTex, noiseUV).r - 0.5;
                
    float NdotH = dot(normal, halfDir);
    float TdotH = dot(IN.tangent, halfDir);
    float NdotV = max(0.0, dot(normal, viewDir));
	float specAttenuation = sqrt(max(0.0, halfLambert / NdotV));
    specAttenuation = clamp(specAttenuation, 0.0 ,1.0) * atten;

    vec3 specCol1 = vec3(0.02, 0.02, 0.02) + albedo.rgb;
    vec3 specOffset1 = normal * (noise * 1 - 0.2);
    vec3 binormalDir1 = normalize(IN.binormal + specOffset1);
    float BdotH1 = dot(binormalDir1, halfDir) / 0.2;
    float specularDis1 = exp(-(TdotH * TdotH + BdotH1 * BdotH1) / (1.0 + NdotH));
    vec3 specularCol1 = specularDis1 * lightColour.rgb * specCol1 * specAttenuation;

    vec3 specCol2 = vec3(0.06, 0.06, 0.06) + albedo.rgb;
    vec3 specOffset2 = normal * (noise * 0.5 - 0.25);
    vec3 binormalDir2 = normalize(IN.binormal + specOffset2);
    float BdotH2 = dot(binormalDir2, halfDir) / 0.1;
    float specularDis2 = exp(-(TdotH * TdotH + BdotH2 * BdotH2) / (1.0 + NdotH));
    vec3 specularCol2 = specularDis2 * lightColour.rgb * specCol2 * specAttenuation;
    fragColour.rgb += specularCol1 + specularCol2;
	}
	
	fragColour.rgb = pow(fragColour.rgb, vec3(1.0 / 2.2f));
	fragColour.rgb = ACES_Tonemapping(fragColour.rgb);
	fragColour.a = 1.0;
}
