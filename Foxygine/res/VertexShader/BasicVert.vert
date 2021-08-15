#version 420 core

//Vertex Layout
layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec3 l_Normal;
layout(location = 2) in vec2 l_UV;
layout(location = 3) in vec3 l_Tangent;
layout(location = 4) in vec3 l_Bitangent;

layout(location = 5) in vec4 i_TransRow1;
layout(location = 6) in vec4 i_TransRow2;
layout(location = 7) in vec4 i_TransRow3;
layout(location = 8) in vec4 i_TransRow4;
 

//Out Pass
out vec3 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexUV;
out vec4 lightSpaceFragPos;
out mat3 TBN;

out vec3 modelPosition;

out vec3 vertexPosition_FS_IN;
out vec3 vertexNormal_FS_IN;
out vec2 vertexUV_FS_IN;
out vec4 lightSpaceFragPos_FS_IN;
out mat3 TBN_FS_IN;
out vec3 IBL_FS_IN;



//Uniforms
layout(binding = 17) uniform sampler2D u_Environment;
layout(binding = 16) uniform samplerCube u_Skybox;


uniform int u_RenderedFrames;

uniform mat4 u_ObjectTransform;

uniform mat4 u_LightSpaceMatrix;

uniform mat4 u_CameraWorldToScreen;
uniform vec4 u_CameraPosition;
uniform vec4 u_CameraDirection;


uniform int u_InstancedRendering;



void main() {
	mat4 modelMatrix;
	if(u_InstancedRendering > 0)
		modelMatrix = mat4(i_TransRow1, i_TransRow2, i_TransRow3, i_TransRow4);
	else
		modelMatrix = u_ObjectTransform;

	vertexPosition = (modelMatrix * vec4(l_Position, 1)).xyz;
	gl_Position = u_CameraWorldToScreen * vec4(vertexPosition, 1);

	mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
	vertexNormal = normalMatrix * l_Normal;

	vec3 T = normalize(normalMatrix * l_Tangent);
	vec3 B = normalize(normalMatrix * l_Bitangent);
	vec3 N = normalize(normalMatrix * l_Normal);
	TBN = mat3(T, B, N);

	vertexUV = l_UV;

	lightSpaceFragPos = u_LightSpaceMatrix * vec4(vertexPosition, 1);
	modelPosition = vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);


	vertexPosition_FS_IN = vertexPosition;
	vertexNormal_FS_IN = vertexNormal;
	vertexUV_FS_IN = vertexUV;
	lightSpaceFragPos_FS_IN = lightSpaceFragPos;
	TBN_FS_IN = TBN;
	IBL_FS_IN = texture(u_Skybox, l_Normal).xyz;
}
