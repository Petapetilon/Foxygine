#version 420 core

//Vertex Layout
layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec2 l_UV;
 

//Out Pass
out vec3 vertexPosition;
out vec2 vertexUV;




//Uniforms
uniform int u_RenderedFrames;

uniform mat4 u_ObjectTransform;




void main() {
	vertexPosition = (u_ObjectTransform * vec4(l_Position, 1)).xyz;
	gl_Position = vec4(vertexPosition, 1);
}
