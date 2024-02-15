#version 330 core
uniform sampler2D diffuseTex;
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
void main(void) {
 float shadow = 1.0; // New !
	
	if( IN . shadowProj . w > 0.0) { // New !
		shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , IN.normal )) * 0.9; 
	
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	float sFactor = pow ( rFactor , 80.0 );

	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	vec2 flippedTexCoord = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
	vec4 texColor = texture(diffuseTex, flippedTexCoord);
    	 
	fragColour = texColor;

	fragColour.rgb = pow(fragColour.rgb, vec3(2.2));
	
	fragColour.rgb = fragColour.rgb * 0.05f; //ambient
	
	fragColour.rgb += fragColour.rgb * lightColour.rgb * lambert * shadow * atten; //diffuse light
	
	fragColour.rgb += lightColour.rgb * sFactor * shadow * atten; //specular light
	
	fragColour.rgb = pow(fragColour.rgb, vec3(1.0 / 2.2f));

	fragColour.a = 1.0;
}
