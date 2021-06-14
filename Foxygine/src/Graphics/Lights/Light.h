#pragma once
#include "../../Math/Color.h"
#include "Lighting.h"


class Light {
protected:
	Color color;
	float intensity;

public:

	virtual void GL_SetLightingPasses(int index) {}
	virtual bool GL_FillLightPass(Lighting::LightPass* pass) { return false; }
	virtual void GL_RenderShadowMap() {}
	virtual void SetShadowMapResolution(unsigned int resolution) {}
	virtual bool GetShadowCapabilities() { return false; }
};