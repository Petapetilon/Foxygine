#version 420 core

const float PI = 3.14159265359;
const float PI2 = 6.28318530718;
struct MaterialProps {
	vec4 color;
	vec2 uvScale;
	vec2 uvOffset;
};

out vec4 FragColor;

in vec3 textureCoords;

layout(binding = 17) uniform sampler2D u_ColorTexture;
uniform MaterialProps u_MaterialProps;


void main() {
	vec3 dir = normalize(textureCoords);
	float u = .5 + atan(dir.z, dir.x) / PI2;
	float v = .5 - asin(dir.y) / PI;
	FragColor = vec4(texture(u_ColorTexture, vec2(u, -v) + u_MaterialProps.uvOffset).xyz * u_MaterialProps.color.xyz, 1);
}