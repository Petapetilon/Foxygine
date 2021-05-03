#pragma once
#include <string>
#include <memory>
#include "Camera.h"
#include "SerializedMesh.h"
#include "Material.h"


class Renderer
{
protected:
	std::shared_ptr<SerializedMesh> GL_BufferData;
	std::shared_ptr<Material> material;
	std::shared_ptr<Shader> shader;

	unsigned int GL_IndexBufferObject;
	unsigned int GL_VertexArrayObject;
	unsigned int GL_VertexBufferObject;

public:
	void SetMaterial(std::shared_ptr<Material>);
	std::shared_ptr<Material> GetMaterial();

	virtual void Draw(std::shared_ptr<Camera>) {}
};

