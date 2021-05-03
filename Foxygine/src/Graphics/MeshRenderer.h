#pragma once
#include "Mesh.h"
#include "../GameObject/Component.h"
#include "Renderer.h"
#include "../GameObject/GameObject.h"
#include "Shaders/Shader.h"



class MeshRenderer final : public Component, public Renderer
{
private:
	std::shared_ptr<Mesh> mesh;

public:
	MeshRenderer(const std::string& objFile);
	MeshRenderer();
	~MeshRenderer();

	void SetMesh(std::shared_ptr<Mesh>);


	void Draw(std::shared_ptr<Camera>) override;
};

