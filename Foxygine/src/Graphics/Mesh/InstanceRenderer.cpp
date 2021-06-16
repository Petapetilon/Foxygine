#include "InstanceRenderer.h"
#include "MeshRenderer.h"


InstanceRenderer::InstanceRenderer(MeshRenderer* _mainRenderer)
{
	mainRenderer = _mainRenderer;
}


void InstanceRenderer::OnAttach()
{
	mainRenderer->LinkInstanceRenderer(this);
}


void InstanceRenderer::OnDetach()
{
	mainRenderer->UnlinkInstanceRenderer(this);
}


void InstanceRenderer::OnDestroy()
{
	mainRenderer->UnlinkInstanceRenderer(this);
}


void InstanceRenderer::Update(float deltaTime)
{
	if(mainRenderer)
		mainRenderer->UpdateInstanceRendererData(this);
}


void InstanceRenderer::OnEnable()
{
	mainRenderer->LinkInstanceRenderer(this);
}


void InstanceRenderer::OnDisable()
{
	mainRenderer->UnlinkInstanceRenderer(this);
}
