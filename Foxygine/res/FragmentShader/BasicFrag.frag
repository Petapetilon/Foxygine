#version 420 core

struct MaterialProps {
	vec4 color;
	float specular;
	float glossiness;
	float metallic;
	vec2 uvScale;
	vec2 uvOffset;
};

out vec4 FragColor;


in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;
in vec4 lightSpaceFragPos;
in mat3 TBN;


uniform int u_RenderedFrames;

uniform mat4 u_ObjectTransform;

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


const int MAX_LIGHTS = 128;
uniform int u_NumberLights;
uniform int u_LightType[MAX_LIGHTS];
uniform vec3 u_LightPosition[MAX_LIGHTS];
uniform vec3 u_LightDirection[MAX_LIGHTS];
uniform vec4 u_LightColor[MAX_LIGHTS];
uniform float u_LightIntensity[MAX_LIGHTS];


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

	if(closestDepth == 1) return 1;

	for(int x = -2; x <= 2; ++x)
	{
		for(int y = -2; y <= 2; ++y)
		{
		    float pcfDepth = texture(u_ShadowDepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
		    shadow += currentDepth - bias < pcfDepth ? 1.0 : 0.1;        
		}    
	}
	
	return shadow /= 16.0;
} 


vec3 CalculateDirectionalColor(int index, vec3 normal, vec3 viewDir, vec3 fragmentColor, float specularMod){
	vec3 lightDir = normalize(u_LightDirection[index].xyz);
	//lightDir = normalize(vec3(1, 2, .5));
    // diffuse shading
    float diffuseFalloff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFalloff = pow(max(dot(-viewDir, reflectDir), 0.0), u_MaterialProps.glossiness * 50);
    // combine results
    vec3 diffuseColor  = u_LightColor[index].xyz * diffuseFalloff * fragmentColor;
    vec3 specularColor = u_LightColor[index].xyz * specularFalloff * diffuseFalloff * specularMod;;
    return (diffuseColor + specularColor);
}


void main() {
	//Normal Map
	vec2 adjustedUV = vertexUV * u_MaterialProps.uvScale + u_MaterialProps.uvOffset;

	vec3 mappedVertexNormal = (TBN * normalize(texture2D(u_NormalMap, adjustedUV).rgb * 2.0 - 1.0)) * u_NormTexEnabled;
	mappedVertexNormal += vertexNormal * (1 - u_NormTexEnabled);
	mappedVertexNormal = normalize(mappedVertexNormal);


	//Color Map wih fallback
	//vec3 color = texture2D(u_ColorTexture, adjustedUV).xyz * float(u_ColTexEnabled) + u_MaterialProps.color.xyz * float(1 - u_ColTexEnabled);
	vec3 color = texture2D(u_ColorTexture, adjustedUV).xyz;

	float specular = 1;


	vec3 fragToCam = vertexPosition.xyz - u_CameraPosition.xyz;	
	vec3 viewDir = normalize(fragToCam);

	vec3 composedColor = color * u_AmbientLight.xyz;
	for(int i = 0; i < u_NumberLights; i++){
		composedColor += CalculateDirectionalColor(i, mappedVertexNormal, viewDir, color, specular) * ShadowCalculation();
	}

	FragColor = vec4(composedColor, 1);
}