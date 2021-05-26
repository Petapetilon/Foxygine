#version 420 core

struct MaterialProps {
	vec4 color;
	vec2 uvScale;
	vec2 uvOffset;
};

in vec3 vertexPosition;
in vec2 vertexUV;

layout(binding = 0) uniform sampler2D u_ColorTexture;

uniform MaterialProps u_MaterialProps;

void main(){
	gl_FragColor = u_MaterialProps.color * texture(u_ColorTexture, vertexUV * u_MaterialProps.uvScale + u_MaterialProps.uvOffset);
}