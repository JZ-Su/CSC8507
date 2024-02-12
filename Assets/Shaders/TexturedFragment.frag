#version 330 core
uniform sampler2D diffuseTex;
in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;
void main(void) {
	 vec2 flippedTexCoord = vec2(IN.texCoord.x, 1.0 - IN.texCoord.y);
    	 vec4 texColor = texture(diffuseTex, flippedTexCoord);
    	 fragColour = texColor;
	 fragColour.a = 1.0;
}
