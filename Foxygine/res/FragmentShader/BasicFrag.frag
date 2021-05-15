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
in vec4 lightSpaceFragPos;
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
layout(binding = 8) uniform sampler2D u_ShadowDepthMap;
uniform int u_ColTexEnabled;
uniform float u_NormTexEnabled;
uniform int u_DispTexEnabled;
uniform int u_SpecTexEnabled;
uniform int u_MetTexEnabled;

uniform MaterialProps u_MaterialProps;

//const int MAX_DIR_LIGHTS = 128;
//uniform int u_NumberDirLights;
//uniform vec3 u_LightDirection[MAX_DIR_LIGHTS];
//uniform vec4 u_LightColor[MAX_DIR_LIGHTS];

const int MAX_POINT_LIGHTS = 128;
uniform int u_NumberPointLights;
uniform vec3 u_PointLightPosition[MAX_POINT_LIGHTS];
uniform vec4 u_PointLightColor[MAX_POINT_LIGHTS];
uniform float u_PointLightIntensity[MAX_POINT_LIGHTS];
uniform int u_PointLightFalloffType[MAX_POINT_LIGHTS];

const int MAX_LIGHTS = 128;
uniform int u_NumberLights;
//uniform int u_LightType[MAX_LIGHTS];
//uniform vec3 u_LightPosition[MAX_LIGHTS];
uniform vec3 u_LightDirection[MAX_LIGHTS];
uniform vec4 u_LightColor[MAX_LIGHTS];
//uniform float u_LightIntensity[MAX_LIGHTS];


uniform vec4 u_AmbientLight;



float ShadowCalculation(){
    // perform perspective divide
    vec3 projCoords = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowDepthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	//if(closestDepth == .0)
		//return 1;

	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float bias = .01;
    //return currentDepth - bias < closestDepth  ? 1.0 : 0.1;
	float shadow;
	
	vec2 texelSize = 1.0 / textureSize(u_ShadowDepthMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
		    float pcfDepth = texture(u_ShadowDepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
		    shadow += currentDepth - bias < pcfDepth ? 1.0 : 0.1;        
		}    
	}
	
	return shadow /= 9.0;
} 


vec3 CalculateDirectionalColor(int index, vec3 normal, vec3 viewDir, vec3 fragmentColor, float specularMod){
	vec3 lightDir = normalize(u_LightDirection[index].xyz);
    // diffuse shading
    float diffuseFalloff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFalloff = pow(max(dot(-viewDir, reflectDir), 0.0), u_MaterialProps.glossiness * 50);
    // combine results
    vec3 diffuseColor  = u_LightColor[index].xyz * diffuseFalloff * fragmentColor;
    vec3 specularColor = u_LightColor[index].xyz * specularFalloff * diffuseFalloff * specularMod;
    return (diffuseColor + specularColor);
}


vec3 CalculatePointColor(int index, vec3 normal, vec3 viewDir, vec3 fragmentColor, float specularMod){
	vec3 lightDir = normalize(vertexPosition - u_LightDirection[index].xyz);
    // diffuse shading
    float diffuseFalloff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFalloff = pow(max(dot(viewDir, reflectDir), 0.0), u_MaterialProps.glossiness * 50);


	//quadratic
	if(u_PointLightFalloffType[index] == 0){
		vec3 squareDistance = vertexPosition - u_PointLightPosition[index].xyz;
		float multiplier = u_PointLightIntensity[index] / (squareDistance.x * squareDistance.x + squareDistance.y * squareDistance.y + squareDistance.z * squareDistance.z);
		diffuseFalloff *= multiplier;
		specularFalloff *= multiplier;
	}
	//linear
	else if(u_PointLightFalloffType[index] == 1){
		float multiplier = u_PointLightIntensity[index] / distance(vertexPosition, u_PointLightPosition[index].xyz);
		diffuseFalloff *= multiplier;
		specularFalloff *= multiplier;
	}

	max(diffuseFalloff, 1);
	max(specularFalloff, 1);

    // combine results
    vec3 diffuseColor  = u_PointLightColor[index].xyz * diffuseFalloff * fragmentColor;
    vec3 specularColor = u_PointLightColor[index].xyz * specularFalloff * diffuseFalloff * specularMod;
    return (diffuseColor + specularColor);
}





void main() {
	//Normal Map
	vec3 mappedVertexNormal = (TBN * normalize(texture2D(u_NormalMap, vertexUV).rgb * 2.0 - 1.0)) * u_NormTexEnabled;
	mappedVertexNormal += vertexNormal * (1 - u_NormTexEnabled);
	mappedVertexNormal = normalize(mappedVertexNormal);


	//Color Map wih fallback
	vec3 color = texture2D(u_ColorTexture, vertexUV).xyz * float(u_ColTexEnabled) + u_MaterialProps.color.xyz * float(1 - u_ColTexEnabled);

	float specular = 1;


	vec3 fragToCam = vertexPosition.xyz - u_CameraPosition.xyz;	
	vec3 viewDir = normalize(fragToCam);

	vec3 composedColor = color * u_AmbientLight.xyz;
	for(int i = 0; i < u_NumberLights; i++){
		composedColor += CalculateDirectionalColor(i, mappedVertexNormal, viewDir, color, specular) * ShadowCalculation();
	}


	FragColor = vec4(composedColor, 1);
}