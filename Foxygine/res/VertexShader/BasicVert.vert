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
out mat3 TBN;



//Uniforms
uniform int u_RenderedFrames;

uniform mat4 u_ObjectTransform;
uniform mat4 u_ObjectOrientation;

uniform mat4 u_CameraWorldToScreen;
uniform vec4 u_CameraPosition;
uniform vec4 u_CameraDirection;

uniform int u_NormTexEnabled;



void main() {
	float frames = u_RenderedFrames;
	vertexPosition = (u_ObjectTransform * vec4(l_Position, 1)).xyz;
	gl_Position = u_CameraWorldToScreen * vec4(vertexPosition, 1);


	vertexNormal = (u_ObjectOrientation * vec4(l_Normal, 1)).xyz;
	if(u_NormTexEnabled > 0){
	vec3 T = normalize(vec3(u_ObjectOrientation * vec4(l_Tangent, 0.0)));
	vec3 B = normalize(vec3(u_ObjectOrientation * vec4(l_Bitangent, 0.0)));
	vec3 N = normalize(vec3(u_ObjectOrientation * vec4(l_Normal, 0.0)));
	TBN = mat3(T, B, N);
	}

	vertexUV = l_UV;

	//CameraPosition = u_CameraPosition;
	//CameraDirection = u_CameraDirection;
	//
	//renderedFrames = u_RenderedFrames;
}
