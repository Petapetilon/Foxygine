#version 330 core

layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec3 l_Normal;
layout(location = 2) in vec2 l_UV;
layout(location = 3) in vec3 l_Tangent;
layout(location = 4) in vec3 l_Bitangent;

layout(location = 5) in vec4 i_TransRow1;
layout(location = 6) in vec4 i_TransRow2;
layout(location = 7) in vec4 i_TransRow3;
layout(location = 8) in vec4 i_TransRow4;


uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_ObjectTransform;

uniform int u_InstancedRendering;

void main()
{
	if(u_InstancedRendering > 0)
		gl_Position = u_LightSpaceMatrix * mat4(i_TransRow1, i_TransRow2, i_TransRow3, i_TransRow4) * vec4(l_Position, 1);
	else
		gl_Position = u_LightSpaceMatrix * u_ObjectTransform * vec4(l_Position, 1);
}