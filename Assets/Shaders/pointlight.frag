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

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float smoothstep(float x, float a, float b){
	x = clamp((x -a) / (b - a), 0.0, 1.0);
	return x * x * (3 - 2 * x);
}


float ShadowCalculation(vec3 wrldPos)
{
	float far_plane = 200;
    vec3 fragToLight = wrldPos - shadowPos; 
	float currentDepth=length(fragToLight);
	
	//float bias = 0.05;
	vec3 normal = normalize(texture(normalTex, wrldPos.xy).xyz);
    vec3 lightDir = normalize(lightPos - wrldPos);
    float bias = smoothstep(max(0.1 * (1.0 - dot(normal, lightDir)), 0.05), 1.0, 0.0);
	float shadow = 0.0;
    int samples = 20;
    float viewDistance = length(cameraPos - wrldPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowTex, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;
        if(currentDepth - bias < closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

	return shadow;
}


float lerp(float x, float a, float b){
	return a + x * (b - a);
}

const float PI = 3.14159265359;

vec3 fresnelSchlick(float a, vec3 f)
{
    return f + (1.0 - f) * pow(clamp(1.0 - a, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 n, vec3 h, float r)
{
    float a      = r * r;
    float a2     = a * a;
    float NdotH  = max(dot(n, h), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float x, float rough)
{
    float r = (rough + 1.0);
    float k = (r*r) / 8.0;

    float num   = x;
    float denom = x * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 n, vec3 v, vec3 l, float r)
{
    float NdotV = max(dot(n, v), 0.0);
    float NdotL = max(dot(n, l), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, r);
    float ggx1  = GeometrySchlickGGX(NdotL, r);

    return ggx1 * ggx2;
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
	float atten = smoothstep(dist / lightRadius, 1.0, 0.0);

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
		float shininess = lerp(smoothness, 1.0,  80) * smoothness;

		float lambert  = max (0.0 , dot ( incident , normal ));
		float halfLambert = (lambert + 1.0) * 0.5;
		float rFactor = max (0.0 , dot ( halfDir , normal ));
		float sFactor = pow ( rFactor , shininess);
		sFactor = max(0.0, sFactor);

		vec3 baseCol = albedo.rgb * (1.0 -metal);
		vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal;

		fragColor[0].rgb = baseCol * lightColour.rgb * atten * lambert  * shadow * aoCol;
		fragColor[1].rgb = specCol * lightColour.rgb * sFactor * atten * shadow * aoCol;
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.2){
		float metal = data.r;
		float roughness = data.g;
		float skin = data.b;
		float smoothness = 1.0 - roughness;
		float shininess = lerp(smoothness, 1.0,  80) * smoothness;

		float lambert  = max (0.01 , dot ( incident , normal ));
		float halfLambert = (lambert + 1.0) * 0.5;
		float rFactor = max (0.0 , dot ( halfDir , normal ));
		float sFactor = pow ( rFactor , shininess);

		vec3 baseCol = albedo.rgb * (1.0 -metal);
		vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal;
		vec3 diffuseCol = baseCol.rgb * lightColour.rgb * lambert * atten * shadow;

		float skinX = smoothstep(lambert * atten + 0.4, 0.0, 1.0);
		skinX = min(skinX, shadow);
		vec2 skinUV = vec2(1 - skinX, 0.1);
		vec3 skinCol = texture(skinTex, skinUV).rgb;
		skinCol = pow(skinCol, vec3(2.2));
		vec3 skinDiffCol = skinCol * lightColour.rgb * baseCol * atten;
		if(skin > 0.5){
			vec3 specCol = albedo.rgb * 0.15;
		}

		fragColor[0].rgb = diffuseCol * (1.0 - skin) + skinDiffCol * skin;
		fragColor[1].rgb = specCol * lightColour.rgb * sFactor * atten * shadow;
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.3){	
		float lambert  = max (0.0 , dot ( incident , normal ));
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
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.4){
		float lambert  = max (0.0 , dot ( incident , normal ));
		float halfLambert = (lambert + 1.0) * 0.5;
		float rFactor = max (0.0 , dot ( halfDir , normal ));
		float sFactor = pow ( rFactor , 80.0 );
		float metal = data.r;
		vec3 baseCol = albedo.rgb * (1.0 -metal);
		vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal;

		fragColor[0].rgb = baseCol * lightColour.rgb * lambert * atten * shadow;
		fragColor[1].rgb = specCol * lightColour.rgb * sFactor * atten * shadow;
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.5){
		fragColor[0].rgb = albedo.rgb * atten;
		fragColor[1].rgb = vec3(0.0, 0.0, 0.0);
		if(data.r > 0.9){
			fragColor[0].rgb += vec3(0, 0, 1);
		}
		fragColor[0].a = data.r;
		fragColor[1].a = 0.0;
	}
	if(indexTex.r == 0.6){
		float metal = data.r;
		float roughness = data.g;
		float bright = addTex.r;
		float smoothness = 1.0 - roughness;
		float shininess = lerp(smoothness, 1.0,  80) * smoothness;
		bright = pow(bright, 1) * 5;

		float lambert  = max (0.0 , dot ( incident , normal ));
		float halfLambert = (lambert + 1.0) * 0.5;
		float rFactor = max (0.0 , dot ( halfDir , normal ));
		float sFactor = pow ( rFactor , shininess);

		vec3 baseCol = albedo.rgb * (1.0 -metal);
		vec3 specCol = vec3(0.04,0.04,0.04) * (1.0 - metal) + albedo.rgb * metal;
		vec3 brightCol = vec3(0.9, 0.6, 0.1) * bright;

		fragColor[0].rgb = baseCol * lightColour.rgb * lambert * atten * shadow + brightCol;
		fragColor[1].rgb = specCol * lightColour.rgb * sFactor * atten * shadow;
		fragColor[0].a = 1.0;
		fragColor[1].a = 0.0;
	}
}