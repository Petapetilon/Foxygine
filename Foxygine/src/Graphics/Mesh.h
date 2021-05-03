#pragma once
#include <memory>
#include <vector>
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include <string>
#include "SerializedMesh.h"


class Mesh
{
private:
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::unique_ptr<Vector3[]> tangents;
	std::unique_ptr<Vector3[]> bitangents;
	std::vector<unsigned int> indices;

public:
	Mesh(const std::string& filePath);
	Mesh();
	Mesh* ImportMesh(const std::string& filePath);
	std::shared_ptr<SerializedMesh> SerializeMeshData();

private:
	
};

