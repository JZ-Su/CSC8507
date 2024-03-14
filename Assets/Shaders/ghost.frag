#version 330 core
uniform sampler2D mainTex;
uniform vec3	cameraPos;
uniform vec3	shadowPos;

in Vertex {
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 fragColor[5];

void main(void) {

	vec2 flippedTexCoord = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
    vec3 normal = IN.normal;	
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );

	float fresnel = max(0.0, dot(normal, viewDir));
	float texColor = texture(mainTex, flippedTexCoord).r + 0.4;
	fresnel = min(texColor, fresnel);
	fresnel = 1.0 - fresnel;
	fresnel = pow(fresnel, 2) * 3;
	
	vec3 mainColor =  vec3(0.1, 0.7, 0.9);
	vec3 insideColor = vec3(0.7, 0.9, 1);

	vec3 color = mainColor * fresnel;
	color = mix(insideColor,mainColor,fresnel);
	color= clamp(color, 0.0, 1.0);
	fresnel = clamp(fresnel, 0.3, 1.0);

	fragColor[0] = vec4(color, fresnel);
	if(fresnel > 0.9){
		fragColor[0].rgb += vec3(0.0, 0.0, 0.5);
	}

	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(fresnel, 0.0, 0.0, 1.0);
	fragColor[3] = vec4(0.0, 0.0, 0.0, 1.0);
	fragColor[4] = vec4(0.5, 0.0, 2.0, 1.0);
}
