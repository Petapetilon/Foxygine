#include "Mesh.h"
#include "OBJLoader.h"
#include "GL.h"
#include <math.h>
#include <fstream>
#include <iostream>


Mesh::Mesh(const std::string& filePath)
{
	ImportMesh(filePath);
}

Mesh::Mesh() {}

Mesh* Mesh::ImportMesh(const std::string& filePath)
{
	objl::Loader loader;
	loader.LoadFile(filePath);
	auto verts = loader.LoadedVertices;
	
	for (int i = 0; i < verts.size(); i++) {
		positions.push_back(Vector3(verts[i].Position.X, verts[i].Position.Y, verts[i].Position.Z));
		normals.push_back(Vector3(verts[i].Normal.X, verts[i].Normal.Y, verts[i].Normal.Z));
		uvs.push_back(Vector2(verts[i].TextureCoordinate.X, verts[i].TextureCoordinate.Y));
	}

	indices = loader.LoadedIndices;

	//Tangents and Bitangents for normal mapping
	tangents = std::unique_ptr<Vector3[]>(new Vector3[positions.size()]);
	bitangents = std::unique_ptr<Vector3[]>(new Vector3[positions.size()]);

	for (int i = 0; i < indices.size() - 2; i += 3) {
		auto edge1 = positions[indices[i + 1]] - positions[indices[i]];
		auto edge2 = positions[indices[i + 2]] - positions[indices[i]];
		auto deltaUV1 = uvs[indices[i + 1]] - uvs[indices[i]];
		auto deltaUV2 = uvs[indices[i + 2]] - uvs[indices[i]];

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		Vector3 tangent;
		Vector3 bitangent;

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		tangents[indices[i]] = tangent;
		tangents[indices[i + 1]] = tangent;
		tangents[indices[i + 2]] = tangent;

		bitangents[indices[i]] = bitangent;
		bitangents[indices[i + 1]] = bitangent;
		bitangents[indices[i + 2]] = bitangent;
	}

	return this;
}

std::shared_ptr<SerializedMesh> Mesh::SerializeMeshData()
{
	SerializedMesh serialzedData;
	serialzedData.serializedIndices = indices;
	for (int i = 0; i < positions.size(); i++) {
		serialzedData.serializedVertexData.push_back(positions[i].x);
		serialzedData.serializedVertexData.push_back(positions[i].y);
		serialzedData.serializedVertexData.push_back(positions[i].z);
								 
		serialzedData.serializedVertexData.push_back(normals[i].x);
		serialzedData.serializedVertexData.push_back(normals[i].y);
		serialzedData.serializedVertexData.push_back(normals[i].z);
								 
		serialzedData.serializedVertexData.push_back(uvs[i].x);
		serialzedData.serializedVertexData.push_back(uvs[i].y);

		serialzedData.serializedVertexData.push_back(tangents[i].x);
		serialzedData.serializedVertexData.push_back(tangents[i].y);
		serialzedData.serializedVertexData.push_back(tangents[i].z);

		serialzedData.serializedVertexData.push_back(bitangents[i].x);
		serialzedData.serializedVertexData.push_back(bitangents[i].y);
		serialzedData.serializedVertexData.push_back(bitangents[i].z);
	}

	serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
	serialzedData.vertexAttribPointerDataSize.push_back(3);

	serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
	serialzedData.vertexAttribPointerDataSize.push_back(3);
	
	serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
	serialzedData.vertexAttribPointerDataSize.push_back(2);

	serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
	serialzedData.vertexAttribPointerDataSize.push_back(3);

	serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
	serialzedData.vertexAttribPointerDataSize.push_back(3);

	serialzedData.vertexAttribPointerCount = 5;

	return std::make_shared<SerializedMesh>(serialzedData);
}
