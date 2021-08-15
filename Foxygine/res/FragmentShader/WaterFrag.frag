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

uniform vec3 u_Time;



void main(){
	float sinTime = sin(u_Time.z);
	float val = fract((vertexUV_FS_IN.x / (vertexUV_FS_IN.y * u_Time.z)/ 1000) * u_Time.x + u_Time.z);
	//vec3 color = u_MaterialProps.color.xyz + .1 * abs(sin(u_Time.z + vertexUV_FS_IN.x * 1000));
	vec3 color = vec3(fract(modf(vertexUV_FS_IN.x / (vertexUV_FS_IN.y + u_Time.x), u_Time.z)) , 0, 0);
	FragColor = vec4(color, u_MaterialProps.color.w);
}