#version 420 core

layout (location = 0) in vec3 l_Position;

out vec3 localPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position =  projection * view * vec4(l_Position, 1.0);
}