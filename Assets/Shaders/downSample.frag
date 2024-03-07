#version 400 core

uniform sampler2D colorTex;
uniform vec2 mapSize;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColor;
//const float scaleFactors[7] = float[](0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006);

void main(void)
{
	vec2 delta = vec2(0, 0);
	delta.x = 1.0 / mapSize.x;
	delta.y = 1.0 / mapSize.y;

    vec4 a = texture2D(colorTex, vec2(IN.texCoord.x - 2 * delta.x, IN.texCoord.y + 2 * delta.y));
    vec4 b = texture2D(colorTex, vec2(IN.texCoord.x, IN.texCoord.y + 2 * delta.y));
    vec4 c = texture2D(colorTex, vec2(IN.texCoord.x + 2 * delta.x, IN.texCoord.y + 2 * delta.y));

    vec4 d = texture2D(colorTex, vec2(IN.texCoord.x - 2 * delta.x, IN.texCoord.y));
    //vec4 e = texture2D(colorTex, vec2(IN.texCoord.x, IN.texCoord.y));
    vec4 e = texture2D(colorTex, IN.texCoord.xy);
    vec4 f = texture2D(colorTex, vec2(IN.texCoord.x + 2 * delta.x, IN.texCoord.y));

    vec4 g = texture2D(colorTex, vec2(IN.texCoord.x - 2 * delta.x, IN.texCoord.y - 2 * delta.y));
    vec4 h = texture2D(colorTex, vec2(IN.texCoord.x, IN.texCoord.y - 2 * delta.y));
    vec4 i = texture2D(colorTex, vec2(IN.texCoord.x + 2 * delta.x, IN.texCoord.y - 2 * delta.y));

    vec4 j = texture2D(colorTex, vec2(IN.texCoord.x - delta.x, IN.texCoord.y + delta.y));
    vec4 k = texture2D(colorTex, vec2(IN.texCoord.x + delta.x, IN.texCoord.y + delta.y));
    vec4 l = texture2D(colorTex, vec2(IN.texCoord.x - delta.x, IN.texCoord.y - delta.y));
    vec4 m = texture2D(colorTex, vec2(IN.texCoord.x + delta.x, IN.texCoord.y - delta.y));

    fragColor = e * 0.125;
    fragColor += (a+c+g+i)*0.03125;
    fragColor += (b+d+f+h)*0.0625;
    fragColor += (j+k+l+m)*0.125;
}