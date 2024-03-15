#version 330 core

uniform sampler2D hdrTex;

in Vertex {
	vec3 viewDir;
} IN;

out vec4 fragColour;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main(void)	{
    vec2 uv = SampleSphericalMap(normalize(IN.viewDir));
    vec3 color = texture(hdrTex, uv).rgb;
    fragColour = vec4(color, 1.0);
}