#version 400 core

uniform sampler2D colorTex;
uniform vec2 mapSize;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void)
{
	vec2 delta = vec2(0, 0);
	delta.x = 1.0 / mapSize.x;
	delta.y = 1.0 / mapSize.y;

    vec3 a = texture2D(colorTex, vec2(IN.texCoord.x - 2 * delta.x, IN.texCoord.y + 2 * delta.y)).rgb;
    vec3 b = texture2D(colorTex, vec2(IN.texCoord.x,               IN.texCoord.y + 2 * delta.y)).rgb;
    vec3 c = texture2D(colorTex, vec2(IN.texCoord.x + 2 * delta.x, IN.texCoord.y + 2 * delta.y)).rgb;

    vec3 d = texture2D(colorTex, vec2(IN.texCoord.x - 2 * delta.x, IN.texCoord.y)).rgb;
    vec4 e = texture2D(colorTex, IN.texCoord.xy);
    vec3 f = texture2D(colorTex, vec2(IN.texCoord.x + 2 * delta.x, IN.texCoord.y)).rgb;

    vec3 g = texture2D(colorTex, vec2(IN.texCoord.x - 2 * delta.x, IN.texCoord.y - 2 * delta.y)).rgb;
    vec3 h = texture2D(colorTex, vec2(IN.texCoord.x,               IN.texCoord.y - 2 * delta.y)).rgb;
    vec3 i = texture2D(colorTex, vec2(IN.texCoord.x + 2 * delta.x, IN.texCoord.y - 2 * delta.y)).rgb;

    vec3 j = texture2D(colorTex, vec2(IN.texCoord.x - delta.x, IN.texCoord.y + delta.y)).rgb;
    vec3 k = texture2D(colorTex, vec2(IN.texCoord.x + delta.x, IN.texCoord.y + delta.y)).rgb;
    vec3 l = texture2D(colorTex, vec2(IN.texCoord.x - delta.x, IN.texCoord.y - delta.y)).rgb;
    vec3 m = texture2D(colorTex, vec2(IN.texCoord.x + delta.x, IN.texCoord.y - delta.y)).rgb;

    fragColor.rgb = e.rgb * 0.125;
    fragColor.rgb += (a+c+g+i)*0.03125;
    fragColor.rgb += (b+d+f+h)*0.0625;
    fragColor.rgb += (j+k+l+m)*0.125;
    fragColor.a = e.a;
}