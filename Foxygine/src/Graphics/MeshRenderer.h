#pragma once
#include "Mesh.h"
#include "../GameObject/Component.h"
#include "Renderer.h"
#include "../GameObject/GameObject.h"
#include "Shaders/Shader.h"


class Material;

class MeshRenderer : public Component, public Renderer
{
private:
	std::shared_ptr<Mesh> mesh;

public:
	MeshRenderer(const std::string& objFile);
	MeshRenderer();
	~MeshRenderer();

	void SetMesh(std::shared_ptr<Mesh> _mesh);

	void Draw(std::shared_ptr<Camera> _drawingCamera) override;
};

