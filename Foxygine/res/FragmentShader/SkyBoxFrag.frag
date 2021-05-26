#version 420 core

struct MaterialProps {
	vec4 color;
	vec2 uvScale;
	vec2 uvOffset;
};

out vec4 FragColor;

in vec3 textureCoords;

layout(binding = 16) uniform samplerCube u_ColorTexture;
uniform MaterialProps u_MaterialProps;


void main() {
	FragColor = vec4(texture(u_ColorTexture, textureCoords).xyz * u_MaterialProps.color.xyz, 1);
}