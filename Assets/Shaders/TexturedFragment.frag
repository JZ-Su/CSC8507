#version 330 core
uniform sampler2D mainTex;
uniform sampler2D normalTex;
uniform sampler2D 	metalTex;
//uniform sampler2DShadow shadowTex;
//uniform samplerCube shadowTex;
//uniform vec3	lightPos;
//uniform float	lightRadius;
//uniform vec4	lightColour;

uniform vec3	cameraPos;

in Vertex {
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 fragColor[5];

//
//float ShadowCalculation(vec3 wrldPos)
//{
//	float far_plane = 200;
//    vec3 fragToLight = wrldPos - lightPos; 
//	float closestDepth = texture(shadowTex, normalize(fragToLight)).r;
//	closestDepth*=far_plane;
//	float currentDepth=length(fragToLight);
//	float bias=0.1;
//	float shadow = currentDepth-bias > closestDepth? 0.0 : 1.0;
//
//	return shadow;
//}

void main(void)
{
	//float shadow = ShadowCalculation(IN.worldPos);

	vec2 flippedTexCoord = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	vec3 normal = texture(normalTex, flippedTexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.xy = normal.xy * 1;
	normal = normalize(TBN * normal);

	//vec3  incident = normalize ( lightPos - IN.worldPos );
	//float lambert  = max (0.0 , dot ( incident , normal ));// * 0.9; 
	//float halfLambert = (lambert + 1.0) * 0.5;
	
	//vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	//vec3 halfDir = normalize ( incident + viewDir );

	//float rFactor = max (0.0 , dot ( halfDir , normal ));
	//float sFactor = pow ( rFactor , 80.0 );

	
	vec4 texColor = texture(mainTex, flippedTexCoord);
	float metal = texture(metalTex, flippedTexCoord).r;

	//texColor.rgb = pow(texColor.rgb, vec3(2.2));
	
//	fragColour.rgb = texColor.rgb * 0.4f * halfLambert; //ambient
//	
//	fragColour.rgb += texColor.rgb * lightColour.rgb * lambert * shadow * atten; //diffuse light
//	
//	fragColour.rgb +=  lightColour.rgb * sFactor * shadow * atten; //specular light
//	
//	fragColour.rgb = pow(fragColour.rgb, vec3(1.0 / 2.2f));
//fragColour.rgb = ACES_Tonemapping(fragColour.rgb);
//
//	fragColour.a = 1.0;

	fragColor[0] = vec4(texColor.rgb, 1.0);
	fragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColor[2] = vec4(metal, 0.0, 0.0, 1.0);
	fragColor[3] = vec4(0.0, 0.0, 0.0, 1.0);
	fragColor[4] = vec4(0.4, 0.0, 0.0, 1.0);
}
