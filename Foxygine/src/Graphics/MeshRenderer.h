#pragma once

#include "Renderer.h"
#include "../GameObject/Component.h"
#include <iostream>
#include <memory>



class Material;
struct SerializedMesh;
class Light;
class Camera;
class Mesh;


class MeshRenderer : public Component, public Renderer
{
protected:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<SerializedMesh> GL_ShadowBufferData;
	bool castShadow;
	bool receiveShadow;

public:
	MeshRenderer(const std::string& objFile, bool _castShadow);
	MeshRenderer();
	~MeshRenderer();

	void SetMesh(std::shared_ptr<Mesh> _mesh);
	void Draw(std::shared_ptr<Camera> _drawingCamera) override;
	void DrawShadowMap(Light* light);
	void OnAttach() override;
	void OnDetach() override;
};

