#version 420 core

struct MaterialProps {
	vec4 color;
	float specular;
	float glossiness;
	float metallic;
};

struct DirectionalLight
{
	vec3 position;
	vec3 direction;
	vec3 color;
};

struct PointLight{
	vec3 position;
	vec3 direction;
	vec3 color;
	float range;
};


out vec4 FragColor;


in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;
in mat3 TBN;


uniform int u_RenderedFrames;

uniform mat4 u_ObjectTransform;
uniform mat4 u_ObjectOrientation;

uniform mat4 u_CameraWorldToScreen;
uniform vec4 u_CameraPosition;
uniform vec4 u_CameraDirection;

layout(binding = 0) uniform sampler2D u_ColorTexture;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_DisplacementMap;
layout(binding = 3) uniform sampler2D u_SpecularMap;
layout(binding = 4) uniform sampler2D u_MetallicMap;
uniform int u_ColTexEnabled;
uniform int u_NormTexEnabled;
uniform int u_DispTexEnabled;
uniform int u_SpecTexEnabled;
uniform int u_MetTexEnabled;

uniform MaterialProps u_MaterialProps;
uniform DirectionalLight u_DirectionalLights[8];
uniform PointLight u_PointLight[32];

void main() {

//Normal Map with fallback
	vec3 mappedVertexNormal;
	if(u_NormTexEnabled > 0){
		vec3 normalMapPoint = texture(u_NormalMap, vertexUV).rgb;
		mappedVertexNormal = normalize(TBN * normalize(normalMapPoint * 2.0 - 1.0)); 
	}
	else
		mappedVertexNormal = normalize(vertexNormal);

	
	
	vec3 lightDirection = normalize(vec3(1, 0, -1));	
	vec3 ambientLight = vec3(.05, .1, .15);
	vec3 directionalLight = vec3(1, .85, .7);
	
	
	//Diffuse part
	float diffuseIntensity =  clamp(0, max(0, dot(mappedVertexNormal, -lightDirection)), 1);
	
	//Highlight cheecky pbr cheat
	vec3 fragToCam = u_CameraPosition.xyz + vertexPosition.xyz;
	vec3 viewDir = normalize(fragToCam);
	vec3 viewReflect = reflect(viewDir, mappedVertexNormal);
	float specularFalloff = clamp(0, pow(dot(viewReflect, -lightDirection), u_MaterialProps.glossiness * 50), 1);

	float metallicFalloff = (clamp(-1, pow(dot(viewReflect, -viewDir), u_MaterialProps.metallic * 50), 1) + 1) * .5;
	


	
	//Composite
	vec3  light = ambientLight
	+ directionalLight.xyz * diffuseIntensity * (1 - u_MaterialProps.glossiness * u_MaterialProps.glossiness)//Phong pbr cheat; darker for high gloss and metallic
	+ vec3(1, 1, 1) * specularFalloff * u_MaterialProps.glossiness;										//Specular pbr cheat; highlight scales with glossiness 
	- vec3(1, 1, 1) * metallicFalloff * u_MaterialProps.metallic;										//Metallic pbr cheat; edges get darker 

	vec3 color = u_MaterialProps.color.xyz;
	if(u_ColTexEnabled > 0){
		color = color * texture2D(u_ColorTexture, vertexUV).xyz;
		//color = vec3(0, 0, 0);
	}		

	FragColor = vec4(light * color, 1); 
}