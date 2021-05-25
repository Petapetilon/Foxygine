#version 420 core

//Vertex Layout
layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec3 l_Normal;
layout(location = 2) in vec2 l_UV;
layout(location = 3) in vec3 l_Tangent;
layout(location = 4) in vec3 l_Bitangent;
 

//Out Pass
out vec3 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexUV;
out vec4 lightSpaceFragPos;
out mat3 TBN;



//Uniforms
uniform int u_RenderedFrames;

uniform mat4 u_ObjectTransform;

uniform mat4 u_LightSpaceMatrix;

uniform mat4 u_CameraWorldToScreen;
uniform vec4 u_CameraPosition;
uniform vec4 u_CameraDirection;



void main() {
	vertexPosition = (u_ObjectTransform * vec4(l_Position, 1)).xyz;
	gl_Position = u_CameraWorldToScreen * vec4(vertexPosition, 1);
	//gl_Position = u_LightSpaceMatrix * vec4(vertexPosition, 1);

	mat3 normalMatrix = mat3(transpose(inverse(u_ObjectTransform)));
	vertexNormal = normalMatrix * l_Normal;

	vec3 T = normalize(normalMatrix * l_Tangent);
	vec3 B = normalize(normalMatrix * l_Bitangent);
	vec3 N = normalize(normalMatrix * l_Normal);
	TBN = mat3(T, B, N);

	vertexUV = l_UV;

	lightSpaceFragPos = u_LightSpaceMatrix * vec4(vertexPosition, 1);
}
