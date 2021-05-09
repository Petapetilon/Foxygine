#version 420 core

struct MaterialProps {
	vec4 color;
	float specular;
	float glossiness;
	float metallic;
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

uniform vec4 u_DirLightDirection;
uniform vec4 u_DirLightColor;

uniform vec3 u_PointLightPosition[32];
uniform vec3 u_PointLightColor[32];

uniform vec4 u_AmbientLight;





vec3 CalculateDirectionalColor(int index, vec3 normal, vec3 viewDir, vec3 fragmentColor, float specularMod){
	vec3 lightDir = normalize(-u_DirLightDirection/*[index]*/.xyz);
    // diffuse shading
    float diffuseFalloff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFalloff = pow(max(dot(viewDir, reflectDir), 0.0), u_MaterialProps.glossiness * 50);
    // combine results
	vec3 ambientColor = u_AmbientLight.xyz * fragmentColor;
    vec3 diffuseColor  = u_DirLightColor/*[index]*/.xyz * diffuseFalloff * fragmentColor;
    vec3 specularColor = u_DirLightColor/*[index]*/.xyz * specularFalloff * diffuseFalloff * specularMod;
    return (ambientColor + diffuseColor + specularColor);
}





void main() {
//Normal Map with fallback
	vec3 mappedVertexNormal;
	if(u_NormTexEnabled > 0){
		vec3 normalMapPoint = texture2D(u_NormalMap, vertexUV).rgb;
		mappedVertexNormal = normalize(TBN * normalize(normalMapPoint * 2.0 - 1.0)); 
	}
	else
		mappedVertexNormal = normalize(vertexNormal);

	//Color Map wih fallback
	vec3 color = u_MaterialProps.color.xyz;
	if(u_ColTexEnabled > 0){
		color = color * texture2D(u_ColorTexture, vertexUV).xyz;
	}

	//Specular Map with fallback
	float specular = 1;
	if(u_SpecTexEnabled > 0){
		specular = pow(texture2D(u_SpecularMap, vertexUV).x, 20);
	}

	vec3 fragToCam = vertexPosition.xyz - u_CameraPosition.xyz;	
	vec3 viewDir = normalize(fragToCam);

	FragColor = vec4(CalculateDirectionalColor(0, mappedVertexNormal, viewDir, color, specular), 1);
	
	//vec3 lightDirection = normalize(vec3(1, 0, -1));	
	//vec3 ambientLight = vec3(.05, .1, .15);
	//vec3 directionalLight = vec3(1, .85, .7);
	//
	//
	////Diffuse part
	//float diffuseIntensity =  clamp(0, max(0, dot(mappedVertexNormal, -lightDirection)), 1);
	//
	////Highlight cheecky pbr cheat
	//vec3 viewReflect = reflect(viewDir, mappedVertexNormal);
	//float specularFalloff = clamp(0, pow(dot(viewReflect, -lightDirection), u_MaterialProps.glossiness * 50), 1);
	//
	//float metallicFalloff = (clamp(-1, pow(dot(viewReflect, -viewDir), u_MaterialProps.metallic * 50), 1) + 1) * .5;
	//
	//
	//
	//
	////Composite
	//vec3  light = ambientLight
	//+ directionalLight.xyz * diffuseIntensity * (1 - u_MaterialProps.glossiness * u_MaterialProps.glossiness)//Phong pbr cheat; darker for high gloss and metallic
	//+ vec3(1, 1, 1) * specularFalloff * u_MaterialProps.glossiness;										//Specular pbr cheat; highlight scales with glossiness 
	//- vec3(1, 1, 1) * metallicFalloff * u_MaterialProps.metallic;										//Metallic pbr cheat; edges get darker 

	//FragColor = vec4(light * color, 1); 
}