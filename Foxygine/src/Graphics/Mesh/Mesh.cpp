#include "Mesh.h"
#include "OBJLoader.h"
#include "../GL.h"
#include <math.h>
#include <fstream>
#include <iostream>


Mesh::Mesh(const std::string& filePath)
{
	ImportMesh(filePath);
}


Mesh::Mesh() 
{ 
	dataIncludeMask = 0; 
}


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
	RecalculateNormals();
	RecalculateBounds();

	IncludeMeshData(MeshData::Everything);
	return this;
}


std::shared_ptr<SerializedMesh> Mesh::SerializeMeshData()
{
	SerializedMesh serialzedData;


	if (dataIncludeMask & (char)MeshData::Indices) {
		serialzedData.serializedIndices = indices;
	}

	for (int i = 0; i < positions.size(); i++) {
		if (dataIncludeMask & (char)MeshData::Positions) {
			serialzedData.serializedVertexData.push_back(positions[i].x);
			serialzedData.serializedVertexData.push_back(positions[i].y);
			serialzedData.serializedVertexData.push_back(positions[i].z);
		}

		if (dataIncludeMask & (char)MeshData::Normals) {
			serialzedData.serializedVertexData.push_back(normals[i].x);
			serialzedData.serializedVertexData.push_back(normals[i].y);
			serialzedData.serializedVertexData.push_back(normals[i].z);
		}

		if (dataIncludeMask & (char)MeshData::UVs) {
			serialzedData.serializedVertexData.push_back(uvs[i].x);
			serialzedData.serializedVertexData.push_back(uvs[i].y);
		}

		if (dataIncludeMask & (char)MeshData::Tangents) {
			serialzedData.serializedVertexData.push_back(tangents[i].x);
			serialzedData.serializedVertexData.push_back(tangents[i].y);
			serialzedData.serializedVertexData.push_back(tangents[i].z);
		}

		if (dataIncludeMask & (char)MeshData::BiTangengs) {
			serialzedData.serializedVertexData.push_back(bitangents[i].x);
			serialzedData.serializedVertexData.push_back(bitangents[i].y);
			serialzedData.serializedVertexData.push_back(bitangents[i].z);
		}
	}

	int includedData = 0;
	if (dataIncludeMask & (char)MeshData::Positions) {
		serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
		serialzedData.vertexAttribPointerDataSize.push_back(3);
		includedData++;
	}

	if (dataIncludeMask & (char)MeshData::Normals) {
		serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
		serialzedData.vertexAttribPointerDataSize.push_back(3);
		includedData++;
	}

	if (dataIncludeMask & (char)MeshData::UVs) {
		serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
		serialzedData.vertexAttribPointerDataSize.push_back(2);
		includedData++;
	}

	if (dataIncludeMask & (char)MeshData::Tangents) {
		serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
		serialzedData.vertexAttribPointerDataSize.push_back(3);
		includedData++;
	}

	if (dataIncludeMask & (char)MeshData::BiTangengs) {
		serialzedData.vertexAttribPointerDataType.push_back(GL_FLOAT);
		serialzedData.vertexAttribPointerDataSize.push_back(3);
		includedData++;
	}

		serialzedData.vertexAttribPointerCount = includedData;

	return std::make_shared<SerializedMesh>(serialzedData);
}


void Mesh::ExcludeMeshData(MeshData excludedData)
{
	dataIncludeMask = dataIncludeMask & !(char)excludedData;
}


void Mesh::IncludeMeshData(MeshData includedData)
{
	dataIncludeMask = dataIncludeMask | (char)includedData;
}


void Mesh::SetPositions(const std::vector<Vector3> _positions)
{
	positions = _positions;
	IncludeMeshData(MeshData::Positions);
}


void Mesh::ReplacePosition(unsigned int index, Vector3 position)
{
	if(index < positions.size())
		positions[index] = position;
}


void Mesh::AppendPosition(Vector3 position)
{
	positions.push_back(position);
}


void Mesh::ClearPositions()
{
	positions.clear();
}


void Mesh::SetNormals(const std::vector<Vector3> _normals)
{
	normals = _normals;
	IncludeMeshData(MeshData::Normals);
}


void Mesh::ReplaceNormal(unsigned int index, Vector3 normal)
{
	if (index < normals.size())
		normals[index] = normal;
}


void Mesh::AppendNormal(Vector3 normal)
{
	normals.push_back(normal);
}


void Mesh::ClearNormals()
{
	normals.clear();
}


void Mesh::SetUVs(const std::vector<Vector2> _uvs)
{
	uvs = _uvs;
	IncludeMeshData(MeshData::UVs);
}


void Mesh::ReplaceUV(unsigned int index, Vector2 uv)
{
	if (index < uvs.size())
		uvs[index] = uv;
}


void Mesh::AppendUV(Vector2 uv)
{
	uvs.push_back(uv);
}


void Mesh::ClearUVs()
{
	uvs.clear();
}


void Mesh::SetIndices(const std::vector<unsigned int> _indices)
{
	indices = _indices;
	IncludeMeshData(MeshData::Indices);
}


void Mesh::ReplaceIndex(unsigned int indexPosition, unsigned int newIndex)
{
	if (indices.size() < indexPosition)
		indices[indexPosition] = newIndex;
}


void Mesh::AppendIndices(unsigned int a, unsigned int b, unsigned c)
{
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
}


void Mesh::ClearIndices()
{
	indices.clear();
}


void Mesh::RecalculateNormals()
{
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

	IncludeMeshData(MeshData::NormalData);
}


void Mesh::RecalculateBounds()
{
}
