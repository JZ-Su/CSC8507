#version 400 core

uniform sampler2D 	colorTex;
uniform sampler2D 	bloomTex;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColor;
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

void main(void)
{
    vec4 hdrColor = texture(colorTex, IN.texCoord);      
    vec4 bloomColor = texture(bloomTex, IN.texCoord);
    hdrColor += bloomColor; // additive blending
   
    fragColor = hdrColor;
	//fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	fragColor.rgb = ACES_Tonemapping(fragColor.rgb);
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
}