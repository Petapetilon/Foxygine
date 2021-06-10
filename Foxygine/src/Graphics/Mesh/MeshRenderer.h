#pragma once

#include "../Rendering/Renderer.h"
#include "../../GameObject/Component.h"
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
	bool castShadow;
	bool receiveShadow;


public:
	MeshRenderer(const std::string& objFile, bool _castShadow);
	MeshRenderer();
	~MeshRenderer();

	virtual void SetMesh(std::shared_ptr<Mesh> _mesh);
	virtual void Draw(std::shared_ptr<Camera> _drawingCamera) override;
	virtual void DrawShadowMap(Light* light);
	virtual void OnAttach() override;
	virtual void OnDetach() override;
};

