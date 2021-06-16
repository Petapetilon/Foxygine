#pragma once

#include "../Rendering/Renderer.h"
#include "../../GameObject/Component.h"
#include <iostream>
#include <memory>
#include <map>



class Material;
struct SerializedMesh;
class Light;
class Camera;
class Mesh;
class InstanceRenderer;


class MeshRenderer : public Component, public Renderer
{
protected:
	std::shared_ptr<Mesh> mesh;
	bool castShadow;
	bool receiveShadow;

	unsigned int GL_InstanceDataBufferObject;
	float* GL_InstanceBufferData;
	std::map<long, InstanceRenderer*> instanceRenderers;


public:
	MeshRenderer(const std::string& objFile, bool _castShadow);
	MeshRenderer();

	virtual void SetMesh(std::shared_ptr<Mesh> _mesh);
	virtual void Draw(std::shared_ptr<Camera> _drawingCamera) override;
	virtual void DrawShadowMap(Light* light);
	virtual void LinkInstanceRenderer(InstanceRenderer* renderer);
	virtual void UnlinkInstanceRenderer(InstanceRenderer* renderer);
	virtual void UpdateInstanceRendererData(InstanceRenderer* renderer);

	virtual Component* Copy(std::size_t& compHash) override;
	virtual Component* CopyLinked(std::size_t& compHash) override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnDestroy() override;
};

