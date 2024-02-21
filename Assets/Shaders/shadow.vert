#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mvpMatrix = mat4(1.0f);

void main()
{
    gl_Position = mvpMatrix * vec4(aPos, 1.0);
} 