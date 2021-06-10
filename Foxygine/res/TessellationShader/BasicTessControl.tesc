#version 420 core


layout(vertices = 3) out;

in vec3 vertexPosition[];
in vec3 vertexNormal[];
in vec2 vertexUV[];
in vec4 lightSpaceFragPos[];
//in mat3 TBN[];

in vec3 modelPosition[];


out vec3 vertexPosition_CS_Out[];
out vec3 vertexNormal_CS_Out[];
out vec2 vertexUV_CS_Out[];
out vec4 lightSpaceFragPos_CS_Out[];
//out mat3 TBN_CS_Out[];

out vec3 sSNormal[];
out vec3 gSNormal[];


uniform vec4 u_CameraPosition;
uniform mat4 u_CameraWorldToScreen;



float LODFactor(float dist){
	return max(0, 10 / pow(dist, 1.6));
}


void CalculateLODGlobalSpace(){
	vec3 center = (vertexPosition[0] + vertexPosition[1] + vertexPosition[2]) * .3333;
	float mult = max(0, dot(normalize(u_CameraPosition.xyz - center), (vertexNormal[0] + vertexNormal[1] + vertexNormal[2]) * .3333));
	

	gl_TessLevelOuter[0] = mix(1, gl_MaxTessGenLevel, LODFactor(distance(vertexPosition[0], u_CameraPosition.xyz)));
	gl_TessLevelOuter[1] = mix(1, gl_MaxTessGenLevel, LODFactor(distance(vertexPosition[1], u_CameraPosition.xyz)));
	gl_TessLevelOuter[2] = mix(1, gl_MaxTessGenLevel, LODFactor(distance(vertexPosition[2], u_CameraPosition.xyz)));
													  
	gl_TessLevelInner[0] = mix(1, gl_MaxTessGenLevel, LODFactor(distance(center, u_CameraPosition.xyz)));
}


void main()
{
	vertexPosition_CS_Out[gl_InvocationID] = vertexPosition[gl_InvocationID];
	vertexNormal_CS_Out[gl_InvocationID] = vertexNormal[gl_InvocationID];
	vertexUV_CS_Out[gl_InvocationID] = vertexUV[gl_InvocationID];
	lightSpaceFragPos_CS_Out[gl_InvocationID] = lightSpaceFragPos[gl_InvocationID];
	//TBN_CS_Out[gl_InvocationID] = TBN[gl_InvocationID];

	sSNormal[gl_InvocationID] = normalize(mat3(u_CameraWorldToScreen) * vertexNormal[gl_InvocationID]);
	//sSNormal[gl_InvocationID] = normalize((vertexPosition[gl_InvocationID] - modelPosition[gl_InvocationID]));
	//gSNormal[gl_InvocationID] = normalize(vertexPosition[gl_InvocationID] - modelPosition[gl_InvocationID]);


	if(gl_InvocationID == 0){
		CalculateLODGlobalSpace();
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
