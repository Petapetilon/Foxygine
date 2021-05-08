#version 330 core

//Vertex Layout
layout(location = 0) in vec3 l_Position;
 
//Out Pass
out vec3 textureCoords;

//Uniforms
uniform mat4 u_CameraWorldToScreen;

void main() {
	textureCoords = l_Position;
	gl_Position = vec4(u_CameraWorldToScreen * vec4(l_Position, 1)).xyww;
}