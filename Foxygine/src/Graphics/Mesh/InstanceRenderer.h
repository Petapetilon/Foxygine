#pragma once
#include "../../GameObject/Component.h"


class MeshRenderer;


class InstanceRenderer : public Component
{
private:
	InstanceRenderer();

public:
	MeshRenderer* mainRenderer;
	unsigned short instanceIndex;

	InstanceRenderer(MeshRenderer* mainRenderer);

	void OnAttach() override;
	void OnDetach() override;
	void OnDestroy() override;
	void Update(float deltaTime) override;
	void OnEnable() override;
	void OnDisable() override;
};

