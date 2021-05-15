#version 330 core

layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec3 l_Normal;
layout(location = 2) in vec2 l_UV;
layout(location = 3) in vec3 l_Tangent;
layout(location = 4) in vec3 l_Bitangent;


uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_ObjectTransform;
uniform mat4 u_CameraWorldToScreen;

out vec3 pos;
out vec2 uv;

void main()
{
	gl_Position = u_LightSpaceMatrix * u_ObjectTransform * vec4(l_Position, 1);
	pos = l_Position;
	uv = l_UV;
}