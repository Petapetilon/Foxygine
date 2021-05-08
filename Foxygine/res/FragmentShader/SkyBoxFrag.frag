#version 420 core

struct MaterialProps {
	vec4 color;
};

out vec4 FragColor;

in vec3 textureCoords;

layout(binding = 0) uniform samplerCube u_ColorTexture;
uniform MaterialProps u_MaterialProps;


void main() {
	FragColor = vec4(texture(u_ColorTexture, textureCoords).xyz * u_MaterialProps.color.xyz, 1);
	//FragColor = texture(u_ColorTexture, textureCoords);
	//FragColor = vec4(1, 0, 1, 1);
}