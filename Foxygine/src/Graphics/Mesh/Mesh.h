#pragma once
#include <memory>
#include <vector>
#include "../../Math/Vector3.h"
#include "../../Math/Vector2.h"
#include <string>
#include "SerializedMesh.h"

//TODO submesh support


class Mesh
{
private:
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::unique_ptr<Vector3[]> tangents;
	std::unique_ptr<Vector3[]> bitangents;
	std::vector<unsigned int> indices;

	char dataIncludeMask;

public:
	enum class MeshData {
		Positions		= 0b000001,
		Normals			= 0b000010,
		UVs				= 0b000100,
		Tangents		= 0b001000,
		BiTangengs		= 0b010000,
		Indices			= 0b100000,

		RawMeshPoint	= 0b100001,
		NormalData		= 0b011100,
		TangentData		= 0b011000,
		VertexData		= 0b000111,
		Everything		= 0b111111,
		Nothing			= 0b000000,
	};

	Mesh(const std::string& filePath);
	Mesh();
	Mesh* ImportMesh(const std::string& filePath);
	std::shared_ptr<SerializedMesh> SerializeMeshData();
	void ExcludeMeshData(MeshData excludedData);
	void IncludeMeshData(MeshData includedData);

	void SetPositions(const std::vector<Vector3> _positions);
	void ReplacePosition(unsigned int index, Vector3 position);
	void AppendPosition(Vector3 position);
	void ClearPositions();

	void SetNormals(const std::vector<Vector3> _normals);
	void ReplaceNormal(unsigned int index, Vector3 normal);
	void AppendNormal(Vector3 normal);
	void ClearNormals();

	void SetUVs(const std::vector<Vector2> _uvs);
	void ReplaceUV(unsigned int index, Vector2 position);
	void AppendUV(Vector2 position);
	void ClearUVs();

	void SetIndices(const std::vector<unsigned int> indices);
	void ReplaceIndex(unsigned int indexPosition, unsigned int newIndex);
	void AppendIndices(unsigned int a, unsigned int b, unsigned c);
	void ClearIndices();

	void RecalculateNormals();
	void RecalculateBounds();	
};

