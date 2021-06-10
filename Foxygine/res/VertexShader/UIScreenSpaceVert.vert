#version 420 core

//Vertex Layout
layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec2 l_UV;
 
//Out Pass
out vec3 vertexPosition;
out vec2 vertexUV;

void main() {
	vertexPosition = l_Position;
	gl_Position = vec4(l_Position, 1);

	vertexUV = l_UV;
}
