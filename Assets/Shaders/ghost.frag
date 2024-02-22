#version 330 core
uniform sampler2D mainTex;
uniform sampler2DShadow shadowTex;
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

void main(void) {

	vec2 flippedTexCoord = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
    vec3 normal = IN.normal;
	vec3  incident = normalize ( lightPos - IN.worldPos );	
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );

	float fresnel = max(0.0, dot(normal, viewDir));
	float texColor = texture(mainTex, flippedTexCoord).r + 0.4;
	fresnel = min(texColor, fresnel);
	fresnel = 1.0 - fresnel;
	fresnel = pow(fresnel, 2) * 3;
	
	vec3 mainColor =  vec3(0.1, 0.7, 0.9);
	vec3 insideColor = vec3(0.7, 0.9, 1);

	vec3 color = mainColor * fresnel;
	color = insideColor * (1.0 - fresnel) + mainColor * fresnel;
	fresnel = clamp(fresnel, 0.3, 1.0);

	fragColour.rgb = color;
	fragColour.a = fresnel;
}
