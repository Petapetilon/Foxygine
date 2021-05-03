#pragma once
#include <vector>


struct SerializedMesh {
	std::vector<unsigned int> serializedIndices;
	std::vector<float> serializedVertexData;

	//GL Vertex Attribute Pointer Information for the serialized Data
	int vertexAttribPointerCount;
	std::vector<int> vertexAttribPointerDataType;
	std::vector<int> vertexAttribPointerDataSize;
};