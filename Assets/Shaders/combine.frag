#version 400 core

uniform sampler2D 	colorTex;
uniform sampler2D 	lightDiffTex;
uniform sampler2D 	lightSpecTex;
uniform sampler2D 	depthTex;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColor[2];

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
	//lightDiff.rgb = pow(lightDiff.rgb, vec3(2.2));
	//lightSpec.rgb = pow(lightSpec.rgb, vec3(2.2));

	fragColor[0].rgb = albedo.rgb * 0.05f * vec3(0.3,0.5,1.0);//* 0.1f * vec3(0.1,0.3,1.0);
	fragColor[0].rgb += lightDiff.rgb;
	fragColor[0].rgb += lightSpec.rgb;
	
	//fragColor[0].rgb = pow(fragColor[0].rgb, vec3(1.0 / 2.2f));
	fragColor[0].a = lightDiff.a;

	fragColor[1] = vec4(0, 0, 0, 0);
	float brightness = max(max(fragColor[0].r, fragColor[0].g), fragColor[0].b);
	if(brightness > 1.0){
		fragColor[1].rgb = fragColor[0].rgb;
		fragColor[1].a = fragColor[0].a;
	}
}