#version 420 core

const float PI = 3.141592;


struct MaterialProps {
	vec4 color;
	float specular;
	float glossiness;
	float roughness;
	float metallic;
	vec2 uvScale;
	vec2 uvOffset;
};

out vec4 FragColor;


in vec3 vertexPosition_FS_IN;
in vec3 vertexNormal_FS_IN;
in vec2 vertexUV_FS_IN;
in vec4 lightSpaceFragPos_FS_IN;
in mat3 TBN_FS_IN;


uniform int u_RenderedFrames;

uniform mat4 u_ObjectTransform;

uniform mat4 u_CameraWorldToScreen;
uniform vec4 u_CameraPosition;
uniform vec4 u_CameraDirection;

layout(binding = 0) uniform sampler2D u_ColorTexture;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 3) uniform sampler2D u_RoughnessMap;
layout(binding = 4) uniform sampler2D u_MetallicMap;
layout(binding = 5) uniform sampler2D u_AOMap;
layout(binding = 8) uniform sampler2D u_ShadowDepthMap;
uniform int u_ColTexEnabled;
uniform float u_NormTexEnabled;
uniform int u_RghTexEnabled;
uniform int u_MetTexEnabled;
uniform int u_AOTexEnabled;

uniform MaterialProps u_MaterialProps;


const int MAX_LIGHTS = 128;
uniform int u_NumberLights;
uniform int u_LightType[MAX_LIGHTS];
uniform vec3 u_LightPosition[MAX_LIGHTS];
uniform vec3 u_LightDirection[MAX_LIGHTS];
uniform vec4 u_LightColor[MAX_LIGHTS];
uniform float u_LightIntensity[MAX_LIGHTS];


uniform vec4 u_AmbientLight;



float DirectionalShadow(int i){
	if(u_LightType[i] > 0) return 1;


    // perform perspective divide
    vec3 projCoords = lightSpaceFragPos_FS_IN.xyz / lightSpaceFragPos_FS_IN.w;
	//TODO do better
	if(abs(projCoords.x) > 1 || abs(projCoords.y) > 1) return 1;
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


float PointShadow(int i){
	return 1;
}


float CalculateShadow(int i){
	switch(u_LightType[i]){
	case 0:
		return DirectionalShadow(i);
	case 1:
		return 1;
	case 2:
		return PointShadow(i);
	}

	return 1;
}


float CalculateAttenuation(int i){
	if(u_LightType[i] < 2)
		return 1.0;
	
	float dist = distance(u_LightPosition[i], vertexPosition_FS_IN);
	return 1.0 / (dist * dist);
}


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
} 


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}


float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}


float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}


void main() {
	//UV
	vec2 adjustedUV = vertexUV_FS_IN * u_MaterialProps.uvScale + u_MaterialProps.uvOffset;

	//Normal Map
	vec3 mappedVertexNormal = (TBN_FS_IN * normalize(texture2D(u_NormalMap, adjustedUV).rgb * 2.0 - 1.0)) * u_NormTexEnabled;
	mappedVertexNormal += vertexNormal_FS_IN * (1 - u_NormTexEnabled);
	mappedVertexNormal = normalize(mappedVertexNormal);

	//Material Param
	vec3 albedo = texture2D(u_ColorTexture, adjustedUV).xyz * u_ColTexEnabled * u_MaterialProps.color.xyz + (1 - u_ColTexEnabled) * u_MaterialProps.color.xyz;
	float roughness = texture(u_RoughnessMap, adjustedUV).x * u_RghTexEnabled + (1 - u_RghTexEnabled) * u_MaterialProps.roughness;
	float metallic = texture(u_MetallicMap, adjustedUV).x * u_MetTexEnabled + (1 - u_MetTexEnabled) * u_MaterialProps.metallic;
	vec3 Lo = vec3(0.0);

	for(int i = 0; i < u_NumberLights; i++){
		vec3 lightPosition = u_LightPosition[i];

		//PBR
		vec3 N = mappedVertexNormal;
		vec3 V = normalize(u_CameraPosition.xyz - vertexPosition_FS_IN);
		vec3 L = normalize(lightPosition - vertexPosition_FS_IN);
		vec3 H = normalize(V + L);

		float attenuation = CalculateAttenuation(i);
		attenuation = 1;
		vec3 radiance = u_LightColor[i].xyz * attenuation;

		vec3 F0 = vec3(0.04); 
		F0      = mix(F0, albedo, metallic);
		vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);


		float NDF = DistributionGGX(N, H, roughness);       
		float G   = GeometrySmith(N, V, L, roughness);

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);  

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
  
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);        
		Lo += (kD * albedo / PI + specular) * radiance * NdotL * CalculateShadow(i);
	}
	
	vec3 color = Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(color, 1.0);
}