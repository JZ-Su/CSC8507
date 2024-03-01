#version 400 core

uniform sampler2D 	depthTex;
uniform sampler2D 	normalTex;
uniform sampler2D 	dataTex;
uniform sampler2D 	colorTex;
uniform sampler2D 	addTex;
uniform sampler2D 	indexTex;
uniform sampler2D 	skinTex;
uniform samplerCube shadowTex;

uniform vec3	lightPos;
uniform vec3	shadowPos;
uniform float	lightRadius;
uniform vec4	lightColour;
uniform mat4 inverseProjView;

uniform vec2 pixelSize;
uniform vec3	cameraPos;

out vec4 fragColor[2];

float ShadowCalculation(vec3 worldPos)
{
	float far_plane = 200;
    vec3 fragToLight = worldPos - shadowPos;//lightPos; 
	float closestDepth = texture(shadowTex, normalize(fragToLight)).r;
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);
	float bias = 0.1;
	float shadow = currentDepth-bias > closestDepth? 0.0 : 1.0;

	return shadow;
}

void main(void)
{

	vec2 texCoord = vec2(gl_FragCoord.xy * pixelSize);
	float depth = texture(depthTex, texCoord.xy ).r;
	vec3 ndcPos = vec3(texCoord, depth) * 2.0 - 1.0;
	vec4 invClipPos = inverseProjView * vec4(ndcPos, 1.0);
	vec3 worldPos = invClipPos.xyz / invClipPos.w;

	vec3  incident = normalize ( lightPos - worldPos );

	vec3 viewDir = normalize ( cameraPos - worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float dist = length(lightPos - worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	if ( atten == 0.0) {
		discard;
	}

	vec3 normal = normalize(texture(normalTex, texCoord.xy).xyz * 2.0 - 1.0);

	vec4 albedo = texture(colorTex, texCoord.xy);
	albedo.rgb = pow(albedo.rgb, vec3(2.2));

	vec4 data = texture(dataTex, texCoord.xy);
	vec4 addTex = texture(addTex, texCoord.xy);
	vec4 indexTex = texture(indexTex, texCoord.xy);
	float shadow = ShadowCalculation(worldPos);

	fragColor[0].rgb = vec3(0.0, 0.0, 0.0);
	fragColor[1].rgb = vec3(0.0, 0.0, 0.0);
	fragColor[0].a = 0.0;
	fragColor[1].a = 1.0;
	
	if(indexTex.r == 0.1){
		float metal = data.r;
		float roughness = data.g;
		vec3 aoCol = addTex.rgb;
		float smoothness = 1.0 - roughness;
		float shininess = (1.0 * (1.0 - smoothness) + 80 * smoothness) * smoothness;

		float lambert  = max (0.0 , dot ( incident , normal ));// * 0.9; 
		float halfLambert = (lambert + 1.0) * 0.5;
		float rFactor = max (0.0 , dot ( halfDir , normal ));
		float sFactor = pow ( rFactor , shininess);

		vec3 baseCol = albedo.rgb * (1.0 -metal);
		vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal;

		fragColor[0].rgb = baseCol * lightColour.rgb * lambert * atten * shadow * aoCol;
		fragColor[1].rgb = specCol * lightColour.rgb * sFactor * atten * shadow * aoCol;
		fragColor[0].rgb = pow(fragColor[0].rgb, vec3(1.0 / 2.2f));
		fragColor[1].rgb = pow(fragColor[1].rgb, vec3(1.0 / 2.2f));
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.2){
		float metal = data.r;
		float roughness = data.g;
		float skin = data.b;
		float smoothness = 1.0 - roughness;
		float shininess = (1.0 * (1.0 - smoothness) + 80 * smoothness) * smoothness;

		float lambert  = max (0.0 , dot ( incident , normal ));// * 0.9; 
		float halfLambert = (lambert + 1.0) * 0.5;
		float rFactor = max (0.0 , dot ( halfDir , normal ));
		float sFactor = pow ( rFactor , shininess);

		vec3 baseCol = albedo.rgb * (1.0 -metal);
		vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal;
		vec3 diffuseCol = baseCol.rgb * lightColour.rgb * lambert * atten * shadow;

		float skinX = clamp(lambert + 0.5, 0.0, 1.0);
		vec2 skinUV = vec2(skinX, 1);
		vec3 skinCol = texture(skinTex, skinUV).rgb;
		skinCol = pow(skinCol, vec3(2.2));
		vec3 skinDiffCol = skinCol * lightColour.rgb * baseCol;

		fragColor[0].rgb = diffuseCol * (1.0 - skin) + skinDiffCol * skin;
		fragColor[1].rgb = specCol * lightColour.rgb * sFactor * atten * shadow;
		fragColor[0].rgb = pow(fragColor[0].rgb, vec3(1.0 / 2.2f));
		fragColor[1].rgb = pow(fragColor[1].rgb, vec3(1.0 / 2.2f));
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.3){	
		float lambert  = max (0.0 , dot ( incident , normal ));// * 0.9; 
		float halfLambert = (lambert + 1.0) * 0.5;

		float noise = indexTex.g;
		vec3 tangent = data.xyz * 2.0 - 1.0;
		vec3 binormal = addTex.xyz * 2.0 - 1.0;

		float NdotH = dot(normal, halfDir);
		float TdotH = dot(tangent, halfDir);
		float NdotV = max(0.0, dot(normal, viewDir));
		float specAttenuation = sqrt(max(0.0, halfLambert / NdotV));
		specAttenuation = clamp(specAttenuation, 0.0 ,1.0) * atten;

   		vec3 specCol1 = vec3(0.02, 0.02, 0.02) + albedo.rgb;
		vec3 specOffset1 = normal * (noise * 1 - 0.2);
		vec3 binormalDir1 = normalize(binormal + specOffset1);
		float BdotH1 = dot(binormalDir1, halfDir) / 0.2;
		float specularDis1 = exp(-(TdotH * TdotH + BdotH1 * BdotH1) / (1.0 + NdotH));
		vec3 specularCol1 = specularDis1 * lightColour.rgb * specCol1 * specAttenuation;

		vec3 specCol2 = vec3(0.06, 0.06, 0.06) + albedo.rgb;
		vec3 specOffset2 = normal * (noise * 0.5 - 0.25);
		vec3 binormalDir2 = normalize(binormal + specOffset2);
		float BdotH2 = dot(binormalDir2, halfDir) / 0.1;
		float specularDis2 = exp(-(TdotH * TdotH + BdotH2 * BdotH2) / (1.0 + NdotH));
		vec3 specularCol2 = specularDis2 * lightColour.rgb * specCol2 * specAttenuation;
		
		fragColor[0].rgb = lightColour.rgb * albedo.rgb * halfLambert * atten;
		fragColor[1].rgb = specularCol1 + specularCol2;
		fragColor[0].rgb = pow(fragColor[0].rgb, vec3(1.0 / 2.2f));
		fragColor[1].rgb = pow(fragColor[1].rgb, vec3(1.0 / 2.2f));
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.4){
		float lambert  = max (0.0 , dot ( incident , normal ));// * 0.9; 
		float halfLambert = (lambert + 1.0) * 0.5;
		float rFactor = max (0.0 , dot ( halfDir , normal ));
		float sFactor = pow ( rFactor , 80.0 );

		fragColor[0].rgb = albedo.rgb * lightColour.rgb * lambert * atten * shadow;
		fragColor[1].rgb = lightColour.rgb * sFactor * atten * shadow;
		fragColor[0].rgb = pow(fragColor[0].rgb, vec3(1.0 / 2.2f));
		fragColor[1].rgb = pow(fragColor[1].rgb, vec3(1.0 / 2.2f));
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.5){
		fragColor[0].rgb = albedo.rgb * atten;
		fragColor[1].rgb = vec3(0.0, 0.0, 0.0);
		fragColor[0].rgb = pow(fragColor[0].rgb, vec3(1.0 / 2.2f));
		fragColor[1].rgb = pow(fragColor[1].rgb, vec3(1.0 / 2.2f));
		fragColor[0].a = data.r;
		fragColor[1].a = 0.0;
	}
}