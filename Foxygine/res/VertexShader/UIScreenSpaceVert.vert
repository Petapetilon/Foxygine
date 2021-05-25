#version 420 core

//Vertex Layout
layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec2 l_UV;
 
//Out Pass
out vec3 vertexPosition;
out vec2 vertexUV;

//Uniforms
//uniform mat4 u_CanvasToScreen;
//uniform mat4 u_ObjectTransform;
//
//uniform vec2 u_PositionOffset;
//uniform vec2 u_Scale;
//uniform vec2 u_WindowSize;


void main() {
	//vertexPosition = (u_ObjectTransform * vec4(l_Position, 1)).xyz;
	//gl_Position = u_CanvasToScreen * vec4(vertexPosition, 1);


	//gl_Position = vec4((l_Position.x * u_Scale.x + u_PositionOffset.x) / u_WindowSize.x, (l_Position.y * u_Scale.y + u_PositionOffset.y) / u_WindowSize.y, 0, 1);
	vertexPosition = l_Position;
	gl_Position = vec4(l_Position, 1);

	vertexUV = l_UV;
}
