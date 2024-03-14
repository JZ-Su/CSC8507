#version 400 core

uniform sampler2D 	colorTex;
uniform sampler2D 	lightDiffTex;
uniform sampler2D 	lightSpecTex;
uniform sampler2D 	depthTex;
uniform sampler2D 	indexTex;
uniform sampler2D irradianceMap;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColor[2];

vec3 fresnelSchlick(float a, vec3 f)
{
    return f + (1.0 - f) * pow(clamp(1.0 - a, 0.0, 1.0), 5.0);
} 

void main(void)
{
	float depth = texture(depthTex, IN.texCoord).r;
	if(depth == 1.0){
		discard;
	}

	vec4 albedo = texture(colorTex, IN.texCoord);
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	vec4 lightDiff = texture(lightDiffTex, IN.texCoord);
	vec4 lightSpec = texture(lightSpecTex, IN.texCoord);
	float halfLambert = texture(indexTex, IN.texCoord).b;
	vec3 ambient = texture(irradianceMap, IN.texCoord).rgb;
	vec3 kS = fresnelSchlick(halfLambert, albedo.rgb);
	vec3 kD = 1.0 - kS;
	
	fragColor[0].a = 1.0;
	if(halfLambert > 1){
		fragColor[0].rgb = albedo.rgb;
		fragColor[0].a = albedo.a;
	}
	else{
		fragColor[0].rgb = halfLambert * ambient * albedo.rgb * 0.15f * vec3(0.5,0.6,1.0);//kD  * vec3(0.3,0.5,1.0);//* 0.1f * vec3(0.1,0.3,1.0);
		fragColor[0].rgb += lightDiff.rgb;
		fragColor[0].rgb += lightSpec.rgb;
	}

	fragColor[1] = vec4(0, 0, 0, 1);
	float brightness = max(max(fragColor[0].r, fragColor[0].g), fragColor[0].b);
	if(brightness > 1.0){
		fragColor[1].rgb = fragColor[0].rgb;
		fragColor[1].a = fragColor[0].a;
	}
}