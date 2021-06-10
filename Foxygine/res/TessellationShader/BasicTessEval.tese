#version 420 core

layout(triangles, fractional_odd_spacing, ccw) in;

in vec3 vertexPosition_CS_Out[];
in vec3 vertexNormal_CS_Out[];
in vec2 vertexUV_CS_Out[];
in vec4 lightSpaceFragPos_CS_Out[];
//in mat3 TBN_CS_Out[];

in vec3 sSNormal[];
in vec3 gSNormal[];

out vec3 vertexPosition_FS_IN;
out vec3 vertexNormal_FS_IN;
out vec2 vertexUV_FS_IN;
out vec4 lightSpaceFragPos_FS_IN;

out float disp;
//out mat3 TBN_FS_IN;



layout(binding = 0) uniform sampler2D u_DisplacementMap;


vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec4 interpolate4D(vec4 v0, vec4 v1, vec4 v2)
{
    return vec4(gl_TessCoord.x) * v0 + vec4(gl_TessCoord.y) * v1 + vec4(gl_TessCoord.z) * v2;
}

void main(){
	vertexUV_FS_IN = interpolate2D(vertexUV_CS_Out[0], vertexUV_CS_Out[1], vertexUV_CS_Out[2]);
	disp = (texture(u_DisplacementMap, vertexUV_FS_IN).r - .5) * .5;

	//vertexPosition_FS_IN = interpolate3D(vertexPosition_CS_Out[0] + displacementFac * gSNormal[0], vertexPosition_CS_Out[1] + displacementFac * gSNormal[1], vertexPosition_CS_Out[2] + displacementFac * gSNormal[2]); 
	//vertexNormal_FS_IN = normalize(cross(vertexPosition_CS_Out[1] + displacementFac * gSNormal[1] - vertexPosition_CS_Out[0] + displacementFac * gSNormal[0], vertexPosition_CS_Out[2] + displacementFac * gSNormal[2] - vertexPosition_CS_Out[0] + displacementFac * gSNormal[0]));
	lightSpaceFragPos_FS_IN = interpolate4D(lightSpaceFragPos_CS_Out[0], lightSpaceFragPos_CS_Out[1], lightSpaceFragPos_CS_Out[2]);
	//TBN_FS_IN = TBN_CS_Out[0];

	gl_Position =	(gl_TessCoord.x * gl_in[0].gl_Position) + 
					(gl_TessCoord.y * gl_in[1].gl_Position) + 
					(gl_TessCoord.z * gl_in[2].gl_Position) +
					//vec4(interpolate3D(sSNormal[0], sSNormal[1], sSNormal[2]), 0) * disp; 
					vec4(interpolate3D(sSNormal[0], sSNormal[1], sSNormal[2]), 0) * disp; 
}
