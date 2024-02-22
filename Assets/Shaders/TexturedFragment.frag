#version 330 core
uniform sampler2D mainTex;
uniform sampler2D normalTex;
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

void main(void) {
 float shadow = 1.0; // New !
	
	if( IN . shadowProj . w > 0.0) { // New !
		shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	}
	vec2 flippedTexCoord = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	vec3 normal = texture(normalTex, flippedTexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , normal ));// * 0.9; 
float halfLambert = (lambert + 1.0) * 0.5;
	
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , normal ));
	float sFactor = pow ( rFactor , 80.0 );

	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	
	vec4 texColor = texture(mainTex, flippedTexCoord);

	texColor.rgb = pow(texColor.rgb, vec3(2.2));
	
	fragColour.rgb = texColor.rgb * 0.4f * halfLambert; //ambient
	
	fragColour.rgb += texColor.rgb * lightColour.rgb * lambert * shadow * atten; //diffuse light
	
	fragColour.rgb +=  lightColour.rgb * sFactor * shadow * atten; //specular light
	
	fragColour.rgb = pow(fragColour.rgb, vec3(1.0 / 2.2f));
fragColour.rgb = ACES_Tonemapping(fragColour.rgb);

	fragColour.a = 1.0;
}
