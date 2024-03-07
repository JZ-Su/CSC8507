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

    vec4 a = texture2D(colorTex, vec2(IN.texCoord.x - delta.x, IN.texCoord.y + delta.y));
    vec4 b = texture2D(colorTex, vec2(IN.texCoord.x, IN.texCoord.y + delta.y));
    vec4 c = texture2D(colorTex, vec2(IN.texCoord.x + delta.x, IN.texCoord.y + delta.y));

    vec4 d = texture2D(colorTex, vec2(IN.texCoord.x - delta.x, IN.texCoord.y));
    vec4 e = texture2D(colorTex, vec2(IN.texCoord.x, IN.texCoord.y));
    vec4 f = texture2D(colorTex, vec2(IN.texCoord.x + delta.x, IN.texCoord.y));

    vec4 g = texture2D(colorTex, vec2(IN.texCoord.x - delta.x, IN.texCoord.y - delta.y));
    vec4 h = texture2D(colorTex, vec2(IN.texCoord.x, IN.texCoord.y - delta.y));
    vec4 i = texture2D(colorTex, vec2(IN.texCoord.x + delta.x, IN.texCoord.y - delta.y));

    fragColor = e * 4.0;
    fragColor += (b+d+f+h)*2.0;
    fragColor += (a+c+g+i);
    fragColor *= 1.0 / 16.0;
}