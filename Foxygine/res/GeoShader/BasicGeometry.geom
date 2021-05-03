#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out float lighting;

uniform mat4 u_CamRot;


void main() {
	vec3 A = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 B = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 calcnormal = normalize(cross(A, B));
	//float light = abs(dot(vec4(calcnormal, 0.0f), (vec4(0, 0, -1.0f, 0.0f))));
	float light = 1;

	gl_Position = gl_in[0].gl_Position;
	lighting = light;
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	lighting = light;
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	lighting = light;
	EmitVertex();
	EndPrimitive();
}